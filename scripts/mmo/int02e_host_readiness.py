#!/usr/bin/env python3
"""Read-only INT-02E host inventory. Never downloads assets or builds Unreal.

Status is preflight evidence, not permission to run an engine build. Pin the
actual Git commit, hydrate LFS independently, and use the INT-02 build gate
only after human review of this JSON report.
"""
from __future__ import annotations

import argparse
from datetime import datetime, timezone
import json
from pathlib import Path
import platform
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[2]
TARGETS = ("MORABEZA.Target.cs", "MORABEZAEditor.Target.cs", "MORABEZAServer.Target.cs")
REQUIRED_PLUGINS = ("EnhancedInput", "Mover", "MoverExamples")
POINTER = b"version https://git-lfs.github.com/spec/v1"
EXPECTED_ASSETS = 293  # Frozen S0 / INT-02C candidate, not a portfolio-wide rule.


def command(args: list[str], cwd: Path, timeout: int = 20) -> tuple[int, str]:
    try:
        result = subprocess.run(args, cwd=cwd, capture_output=True, text=True,
                                check=False, timeout=timeout)
    except (OSError, subprocess.TimeoutExpired):
        return 127, ""
    return result.returncode, result.stdout.strip()


def plugin_paths(project: Path, engine: Path) -> dict[str, str | None]:
    found: dict[str, str | None] = {name: None for name in REQUIRED_PLUGINS}
    for base in (project / "Plugins", engine / "Engine" / "Plugins"):
        if not base.is_dir():
            continue
        for plugin in base.rglob("*.uplugin"):
            name = plugin.stem
            if name in found and found[name] is None:
                found[name] = str(plugin)
        if all(found.values()):
            break
    return found


def is_lfs_pointer(path: Path) -> bool:
    with path.open("rb") as stream:
        return stream.read(len(POINTER)) == POINTER


def inspect(project: Path, engine: Path | None, candidate_sha: str,
            verify_lfs_objects: bool = False) -> dict:
    project = project.resolve()
    engine = engine.resolve() if engine is not None else None
    blockers: list[str] = []
    review: list[str] = []
    host = platform.system()
    result: dict = {
        "schema": "node63.morabezza.int02e.host-readiness.v1",
        "recorded_at_utc": datetime.now(timezone.utc).isoformat(),
        "project_root": str(project), "engine_root": str(engine) if engine else None,
        "host_os": host, "candidate_sha_requested": candidate_sha,
        "builds_executed": 0, "network_actions_executed": 0,
        "production_or_main_authorized": False,
    }
    if host not in ("Linux", "Darwin", "Windows"):
        blockers.append("UNSUPPORTED_BUILD_HOST_OS")
    if len(candidate_sha) != 40 or any(c not in "0123456789abcdef" for c in candidate_sha.lower()):
        blockers.append("CANDIDATE_SHA_NOT_40_HEX")

    git = shutil.which("git")
    has_checkout = (project / ".git").exists()
    result["is_git_checkout"] = has_checkout
    if not has_checkout or not git:
        blockers.append("EXACT_GIT_CHECKOUT_REQUIRED")
    else:
        code, actual = command([git, "rev-parse", "HEAD"], project)
        result["candidate_sha_actual"] = actual if code == 0 else None
        if code != 0 or actual.lower() != candidate_sha.lower():
            blockers.append("PINNED_GIT_HEAD_MISMATCH")
        code, status = command([git, "status", "--porcelain", "--untracked-files=no"], project)
        result["tracked_tree_clean"] = code == 0 and not status
        if not result["tracked_tree_clean"]:
            blockers.append("TRACKED_WORKTREE_NOT_CLEAN_OR_UNREADABLE")
        code, version = command([git, "lfs", "version"], project)
        result["git_lfs_available"] = code == 0
        result["git_lfs_version"] = version if code == 0 else None
        if code != 0:
            blockers.append("GIT_LFS_UNAVAILABLE")
        elif verify_lfs_objects:
            code, _ = command([git, "lfs", "fsck", "--objects", "--dry-run"],
                              project, timeout=600)
            result["git_lfs_fsck_dry_run_pass"] = code == 0
            if code != 0:
                blockers.append("GIT_LFS_OBJECT_FSCK_FAILED")
    result["lfs_fsck_requested"] = verify_lfs_objects

    definition: dict = {}
    try:
        definition = json.loads((project / "MORABEZA.uproject").read_text(encoding="utf-8"))
    except (OSError, ValueError):
        blockers.append("UPROJECT_UNAVAILABLE_OR_INVALID")
    result["uproject_engine_association"] = definition.get("EngineAssociation")
    if definition.get("EngineAssociation") != "5.8":
        blockers.append("PROJECT_NOT_PINNED_TO_UE58")
    enabled = {p.get("Name") for p in definition.get("Plugins", []) if p.get("Enabled")}
    result["enabled_project_plugins"] = sorted(name for name in enabled if isinstance(name, str))
    for name in ("EnhancedInput", "MoverExamples"):
        if name not in enabled:
            blockers.append("PROJECT_PLUGIN_NOT_ENABLED:" + name)
    if "Mover" not in enabled:
        review.append("MOVER_PLUGIN_NOT_EXPLICITLY_ENABLED_VERIFY_EXAMPLES_DEPENDENCY")
    result["targets_present"] = {name: (project / "Source" / name).is_file()
                                 for name in TARGETS}
    for name, present in result["targets_present"].items():
        if not present:
            blockers.append("TARGET_MISSING:" + name)

    assets = [p for p in (project / "Content").rglob("*")
              if p.is_file() and p.suffix.lower() in (".uasset", ".umap")]
    pointers = [p for p in assets if is_lfs_pointer(p)]
    result["tracked_package_expected"] = EXPECTED_ASSETS
    result["package_paths_found"] = len(assets)
    result["lfs_pointer_placeholders"] = len(pointers)
    if len(assets) != EXPECTED_ASSETS:
        blockers.append("FROZEN_ASSET_COUNT_MISMATCH")
    if pointers:
        blockers.append("LFS_PAYLOADS_NOT_HYDRATED:" + str(len(pointers)))

    if engine is None or not engine.is_dir():
        blockers.append("UE58_ENGINE_ROOT_MISSING")
    else:
        try:
            version = json.loads((engine / "Engine" / "Build" / "Build.version").read_text(encoding="utf-8"))
            result["engine_version"] = {k: version.get(k) for k in
                                        ("MajorVersion", "MinorVersion", "PatchVersion", "Changelist")}
            if (version.get("MajorVersion"), version.get("MinorVersion")) != (5, 8):
                blockers.append("ENGINE_VERSION_NOT_5_8")
        except (OSError, ValueError):
            blockers.append("ENGINE_BUILD_VERSION_MISSING_OR_INVALID")
        script_name = {"Linux": "Linux/Build.sh", "Darwin": "Mac/Build.sh",
                       "Windows": "Build.bat"}.get(host)
        if script_name and not (engine / "Engine" / "Build" / "BatchFiles" / script_name).is_file():
            blockers.append("ENGINE_BUILD_ENTRYPOINT_MISSING")
        source_markers = (
            (engine / "Engine" / "Source" / "Runtime").is_dir()
            and (engine / "Engine" / "Source" / "Programs" / "UnrealBuildTool").is_dir()
        )
        result["server_source_build_markers_present"] = source_markers
        if not source_markers:
            blockers.append("SERVER_SOURCE_ENGINE_MARKERS_MISSING")
        else:
            review.append("SOURCE_MARKERS_DO_NOT_PROVE_SERVER_TARGET_BUILD_CAPABILITY")
        plugins = plugin_paths(project, engine)
        result["plugin_descriptors"] = plugins
        for name, path in plugins.items():
            if path is None:
                blockers.append("REQUIRED_PLUGIN_DESCRIPTOR_MISSING:" + name)

    result["compiler_on_path"] = {
        "Linux": shutil.which("clang++"), "Darwin": shutil.which("xcrun"),
        "Windows": shutil.which("cl.exe") or shutil.which("clang-cl.exe"),
    }.get(host)
    if not result["compiler_on_path"]:
        review.append("COMPILER_NOT_ON_PATH_VERIFY_UE_BUNDLED_OR_PLATFORM_TOOLCHAIN")
    review.append("RECORD_ENGINE_COMPILER_SDK_AND_PLUGIN_VERSIONS_ON_REAL_HOST")
    review.append("REVIEW_EDITORGAME_SERVER_BUILD_OUTPUTS_SEPARATELY")
    result["blockers"] = sorted(set(blockers))
    result["manual_review_required"] = sorted(set(review))
    result["status"] = "BLOCKED" if blockers else "HOST_PREFLIGHT_ELIGIBLE_REVIEW_REQUIRED"
    return result


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--project-root", type=Path, default=ROOT)
    parser.add_argument("--engine-root", type=Path)
    parser.add_argument("--candidate-sha", required=True)
    parser.add_argument("--evidence-dir", type=Path)
    parser.add_argument("--verify-lfs-objects", action="store_true",
                        help="Read-only local git lfs fsck --objects --dry-run (may take time)")
    args = parser.parse_args()
    root = args.project_root.resolve()
    report = inspect(root, args.engine_root, args.candidate_sha, args.verify_lfs_objects)
    evidence = (args.evidence_dir or root / "Saved" / "INT02E").resolve()
    evidence.mkdir(parents=True, exist_ok=True)
    report_file = evidence / "int02e-host-readiness.json"
    report_file.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    print("INT-02E:", report["status"])
    print("Blockers:", ", ".join(report["blockers"]) if report["blockers"] else "none")
    print("Evidence:", report_file)
    print("No download, engine build, game launch or remote action was performed.")
    return 2 if report["blockers"] else 0


if __name__ == "__main__":
    raise SystemExit(main())
