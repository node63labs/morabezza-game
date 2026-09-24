#!/usr/bin/env python3
"""INT-02 preflight and opt-in Unreal 5.8 source-target build evidence.

Does not run Unreal unless --execute is supplied. Does not approve runtime or
production promotion. Run from an exact git checkout with Git LFS hydrated.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import platform
import subprocess
import sys
from datetime import datetime, timezone

ROOT = Path(__file__).resolve().parents[2]
TARGETS = ("MORABEZAEditor", "MORABEZA", "MORABEZAServer")
EXPECTED_LFS_PACKAGES = 293  # Frozen S0 integration baseline, not a global rule.


def checked_commit(root: Path) -> str | None:
    if not (root / ".git").exists():
        return None
    result = subprocess.run(
        ["git", "-C", str(root), "rev-parse", "HEAD"],
        text=True, capture_output=True, check=False,
    )
    return result.stdout.strip() if result.returncode == 0 else None


def find_plugin(engine: Path, project: Path, name: str) -> bool:
    for root in (project / "Plugins", engine / "Engine" / "Plugins"):
        if root.is_dir() and next(root.rglob(name + ".uplugin"), None):
            return True
    return False


def run_source_preflight(root: Path) -> tuple[bool, str]:
    verifier = root / "scripts" / "mmo" / "verify_s0_integration.py"
    if not verifier.is_file():
        return False, "Source manifest verifier is missing"
    result = subprocess.run(
        [sys.executable, str(verifier)], cwd=root,
        text=True, capture_output=True, check=False,
    )
    return result.returncode == 0, (result.stdout + result.stderr).strip()[-1600:]


def build_command(build_script: Path, target: str, system: str, uproject: Path) -> list[str]:
    args = [target, system, "Development", "-Project=" + str(uproject), "-WaitMutex"]
    return ["cmd", "/c", str(build_script), *args] if system == "Win64" else [str(build_script), *args]


def write_report(path: Path, report: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--engine-root", type=Path, help="Actual UE 5.8 installation/source root")
    parser.add_argument("--project-root", type=Path, default=ROOT)
    parser.add_argument("--candidate-sha", help="Exact 40-character Git commit of tested source")
    parser.add_argument("--evidence-dir", type=Path, help="Local untracked output directory")
    parser.add_argument("--execute", action="store_true", help="Explicitly compile Editor, Game and Server after all prerequisites pass")
    args = parser.parse_args()

    root = args.project_root.resolve()
    engine = args.engine_root.resolve() if args.engine_root else None
    evidence = (args.evidence_dir or root / "Saved" / "INT02").resolve()
    system = {"Linux": "Linux", "Darwin": "Mac", "Windows": "Win64"}.get(platform.system())
    blockers: list[str] = []
    report: dict = {
        "schema": "node63.morabezza.int02.build-evidence.v1",
        "recorded_at_utc": datetime.now(timezone.utc).isoformat(),
        "project_root": str(root),
        "engine_root": str(engine) if engine else None,
        "host_platform": system,
        "candidate_sha_requested": args.candidate_sha,
        "candidate_sha_actual": checked_commit(root),
        "execute_requested": args.execute,
        "builds": [],
        "status": "PREFLIGHT_PENDING",
        "runtime_verification": "NOT EXECUTED",
        "main_merge_authorized": False,
    }
    uproject = root / "MORABEZA.uproject"
    manifest_ok, manifest_output = run_source_preflight(root)
    report["source_manifest_pass"] = manifest_ok
    report["source_verifier_excerpt"] = manifest_output
    if not manifest_ok:
        blockers.append("SOURCE_MANIFEST_VERIFICATION_FAILED")
    if system is None:
        blockers.append("UNSUPPORTED_HOST_OS")
    if not args.candidate_sha or len(args.candidate_sha) != 40 or any(c not in "0123456789abcdef" for c in args.candidate_sha.lower()):
        blockers.append("EXACT_CANDIDATE_SHA_REQUIRED")
    if not report["candidate_sha_actual"]:
        blockers.append("GIT_COMMIT_NOT_PROVABLE_FROM_SOURCE_ARCHIVE")
    elif args.candidate_sha and report["candidate_sha_actual"] != args.candidate_sha.lower():
        blockers.append("CHECKOUT_SHA_MISMATCH")

    if not uproject.is_file():
        blockers.append("UPROJECT_MISSING")
        definition = {}
    else:
        try:
            definition = json.loads(uproject.read_text(encoding="utf-8"))
        except (ValueError, OSError):
            definition = {}
            blockers.append("UPROJECT_INVALID")
    report["uproject_engine_association"] = definition.get("EngineAssociation")
    if definition.get("EngineAssociation") != "5.8":
        blockers.append("UPROJECT_NOT_UE_5_8")
    required_plugins = [p["Name"] for p in definition.get("Plugins", []) if p.get("Enabled")]
    report["required_plugins"] = required_plugins
    for target in TARGETS:
        if not (root / "Source" / (target + ".Target.cs")).is_file():
            blockers.append("TARGET_MISSING:" + target)

    assets = sorted(p for p in (root / "Content").rglob("*") if p.suffix.lower() in (".umap", ".uasset") and p.is_file())
    placeholders = [p for p in assets if p.read_bytes()[:46].startswith(b"version https://git-lfs.github.com/spec/v1")]
    report["unreal_package_paths"] = len(assets)
    report["lfs_pointer_placeholders"] = len(placeholders)
    if len(assets) != EXPECTED_LFS_PACKAGES:
        blockers.append("FROZEN_PACKAGE_COUNT_MISMATCH")
    if placeholders:
        blockers.append("LFS_PAYLOADS_MISSING:" + str(len(placeholders)))

    build_script = None
    if not engine or not engine.is_dir() or not system:
        blockers.append("UE_ENGINE_ROOT_UNAVAILABLE")
    else:
        build_version = engine / "Engine" / "Build" / "Build.version"
        try:
            version = json.loads(build_version.read_text(encoding="utf-8"))
            report["engine_version"] = {k: version.get(k) for k in ("MajorVersion", "MinorVersion", "PatchVersion", "Changelist")}
            if version.get("MajorVersion") != 5 or version.get("MinorVersion") != 8:
                blockers.append("ENGINE_NOT_UE_5_8")
        except (OSError, ValueError):
            blockers.append("ENGINE_BUILD_VERSION_UNAVAILABLE")
        relative = "Build.bat" if system == "Win64" else ("Mac/Build.sh" if system == "Mac" else "Linux/Build.sh")
        build_script = engine / "Engine" / "Build" / "BatchFiles" / relative
        if not build_script.is_file():
            blockers.append("UNREAL_BUILD_SCRIPT_MISSING")
        for name in required_plugins:
            if not find_plugin(engine, root, name):
                blockers.append("REQUIRED_PLUGIN_MISSING:" + name)

    report["blockers"] = blockers
    report_path = evidence / "int02-build-evidence.json"
    if blockers:
        report["status"] = "BLOCKED_NO_UNREAL_BUILD"
        write_report(report_path, report)
        print("INT-02 BLOCKED: " + ", ".join(blockers))
        print("Evidence: " + str(report_path))
        return 2

    report["status"] = "READY_NOT_COMPILED"
    commands = [build_command(build_script, target, system, uproject) for target in TARGETS]
    report["planned_build_commands"] = commands
    if not args.execute:
        write_report(report_path, report)
        print("INT-02 PREFLIGHT READY — no compile executed (pass --execute explicitly)")
        print("Evidence: " + str(report_path))
        return 0

    for target, command in zip(TARGETS, commands):
        log = evidence / ("build-" + target + ".log")
        log.parent.mkdir(parents=True, exist_ok=True)
        with log.open("wb") as stream:
            try:
                result = subprocess.run(command, cwd=root, stdout=stream, stderr=subprocess.STDOUT, check=False)
                exit_code = result.returncode
            except OSError as exc:
                stream.write(("Build invocation failed: " + str(exc) + "\n").encode())
                exit_code = 127
        record = {"target": target, "command": command, "exit_code": exit_code,
                  "log_path": str(log), "log_sha256": hashlib.sha256(log.read_bytes()).hexdigest()}
        report["builds"].append(record)
        if exit_code != 0:
            report["status"] = "BUILD_FAILED_NOT_ACCEPTED"
            write_report(report_path, report)
            print("INT-02 BUILD FAILED: " + target + "; see " + str(log))
            return 1
        write_report(report_path, report)
    report["status"] = "TARGET_BUILDS_ZERO_EXIT_REVIEW_REQUIRED"
    write_report(report_path, report)
    print("INT-02 BUILDS EXITED ZERO — review UHT, outputs and logs before gate acceptance")
    print("Unreal runtime/two-client verification remains NOT EXECUTED")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
