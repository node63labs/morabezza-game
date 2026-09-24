#!/usr/bin/env python3
"""Verify the frozen S0 PR4–PR7 candidate without GitHub Actions.

Default mode checks exact Git blob identities for the 21 imported/reviewed
text files. --run-tests also runs shell syntax and all offline Python
source-contract tests. Neither mode compiles or starts Unreal Engine.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
MANIFEST = ROOT / "Docs" / "S0_INTEGRATION_SOURCE_MANIFEST.json"
REVIEW_OVERLAY = ROOT / "Docs" / "INT_02C_SOURCE_OVERRIDES.json"


def git_blob_sha1(data: bytes) -> str:
    # Git's SHA-1 blob identity format for this repository. This is a
    # provenance/byte-equality check, not a separate security audit.
    payload = b"blob " + str(len(data)).encode("ascii") + b"\0" + data
    return hashlib.sha1(payload).hexdigest()


def verify_manifest() -> None:
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    if manifest.get("schema") != "node63.morabezza.s0.integration-source-manifest.v1":
        raise ValueError("unknown or missing source manifest schema")
    if manifest.get("integration_base_sha") != manifest["source_snapshot_heads"]["PR-7_S0-W1c"]:
        raise ValueError("integration base and PR7 frozen SHA disagree")
    if manifest["verification_contract"]["main_merge_authorization"] != "NOT GRANTED":
        raise ValueError("this verifier is only for the unaccepted candidate baseline")

    entries = {
        **manifest["paths"]["from_pr4"],
        **manifest["paths"]["from_pr7_stack"],
    }
    if (
        len(manifest["paths"]["from_pr4"]) != 5
        or len(manifest["paths"]["from_pr7_stack"]) != 16
        or len(entries) != 21
    ):
        raise ValueError("unexpected source manifest path count or collision")

    # INT-02C is a distinct reviewed source revision, not a rewrite of
    # INT-01's frozen PR4–PR7 manifest. Strictly layer the three documented
    # pre-build changes onto that immutable source identity.
    if REVIEW_OVERLAY.is_file():
        overlay = json.loads(REVIEW_OVERLAY.read_text(encoding="utf-8"))
        if overlay.get("schema") != "node63.morabezza.int02c.source-overlay.v1":
            raise ValueError("unknown INT-02C source overlay schema")
        if overlay.get("review_base_commit") != "7f92c7a120026c0cb4a3cfaff34c6cbece068e12":
            raise ValueError("INT-02C overlay does not identify the reviewed PR9 base")
        if overlay.get("frozen_s0_manifest_sha") != git_blob_sha1(MANIFEST.read_bytes()):
            raise ValueError("INT-02C overlay's immutable S0 manifest hash changed")
        before = {
            "Source/MORABEZA/MORABEZACharacter.cpp":
                "3636f502a00dc43b80f8d22b13f6aa820d37d863",
            "Source/MORABEZA/LandscapeDiagnosticCommandlet.cpp":
                "7768e6e3a53dc3fbe0fd4676f51ca62931d68133",
            "Source/MORABEZA/LandscapeDiagnosticCommandlet.h":
                "d363f425306d2dd71c80e281fe48ae3879ae66e6",
        }
        changes = overlay.get("overrides", {})
        added = overlay.get("added_paths", {})
        if set(changes) != set(before) or set(added) != {
            "tests/mmo/test_int02c_prebuild_source.py"
        }:
            raise ValueError("INT-02C overlay contains unexpected changed paths")
        for path, original_sha in before.items():
            change = changes[path]
            if change.get("before") != original_sha:
                raise ValueError(f"INT-02C pre-review source SHA mismatch: {path}")
            if path in entries and entries[path] != original_sha:
                raise ValueError(f"INT-02C override disagrees with frozen S0 manifest: {path}")
            entries[path] = change["after"]
        entries.update(added)
        if len(entries) != 24:
            raise ValueError("INT-02C overlay has wrong combined path count")
        print("INT-02C REVIEW OVERLAY ACTIVE: 3 reviewed changes and 1 new regression test")

    errors: list[str] = []
    for raw_path, expected in sorted(entries.items()):
        rel = PurePosixPath(raw_path)
        if rel.is_absolute() or ".." in rel.parts or "\\" in raw_path:
            errors.append(f"unsafe relative path in manifest: {raw_path!r}")
            continue
        if not isinstance(expected, str) or len(expected) != 40:
            errors.append(f"invalid Git blob SHA for {raw_path}")
            continue
        full_path = ROOT.joinpath(*rel.parts)
        if not full_path.is_file():
            errors.append(f"missing file: {raw_path}")
            continue
        actual = git_blob_sha1(full_path.read_bytes())
        if actual != expected:
            errors.append(
                f"blob mismatch: {raw_path} (expected {expected}; actual {actual})"
            )
    if errors:
        raise ValueError("\n".join(errors))

    print(f"S0 SOURCE SNAPSHOT PASS: {len(entries)} exact Git blob IDs")
    print("S0 STATUS: candidate only; Unreal compile and runtime NOT VERIFIED")


def run_offline_tests() -> None:
    if os.name != "posix" or not shutil.which("bash"):
        raise RuntimeError(
            "--run-tests needs a POSIX shell with Bash; use WSL or a Linux/macOS host"
        )

    commands = [
        ["bash", "-n", "scripts/mmo/s0_two_client_smoke.sh"],
        [
            sys.executable, "-m", "unittest", "discover",
            "-s", "tests/mmo", "-p", "test_*.py", "-v",
        ],
    ]
    for cmd in commands:
        print("S0 OFFLINE TEST:", " ".join(cmd), flush=True)
        subprocess.run(cmd, cwd=ROOT, check=True)
    print("S0 OFFLINE TEST PASS: syntax and Python source-contract suites only")
    print("UNREAL COMPILE: NOT EXECUTED; TWO-CLIENT NETWORK TEST: NOT EXECUTED")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--run-tests",
        action="store_true",
        help="also run Bash syntax and offline Python source tests (POSIX only)",
    )
    args = parser.parse_args()
    try:
        verify_manifest()
        if args.run_tests:
            run_offline_tests()
    except (OSError, ValueError, RuntimeError, subprocess.CalledProcessError) as exc:
        print(f"S0 INTEGRATION PRECHECK FAIL: {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
