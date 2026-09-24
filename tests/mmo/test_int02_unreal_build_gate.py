"""INT-02 offline preflight tests: no real engine or network is invoked."""
from __future__ import annotations

import importlib.util
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest import mock

MODULE_PATH = Path(__file__).resolve().parents[2] / "scripts" / "mmo" / "int02_unreal_build_gate.py"
SPEC = importlib.util.spec_from_file_location("int02_unreal_build_gate", MODULE_PATH)
assert SPEC and SPEC.loader
GATE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(GATE)
EXAMPLE_SHA = "a" * 40


class Int02BuildGateTests(unittest.TestCase):
    def test_platform_commands_are_argv_not_shell_code(self):
        project = Path("/tmp/UE Project/MORABEZA.uproject")
        script = Path("/tmp/UE 5.8/Engine/Build/BatchFiles/Linux/Build.sh")
        linux = GATE.build_command(script, "MORABEZAServer", "Linux", project)
        self.assertEqual(linux[:3], [str(script), "MORABEZAServer", "Linux"])
        self.assertIn("-Project=" + str(project), linux)
        self.assertEqual(linux[-1], "-WaitMutex")
        windows = GATE.build_command(script, "MORABEZAEditor", "Win64", project)
        self.assertEqual(windows[:3], ["cmd", "/c", str(script)])
        self.assertIn("MORABEZAEditor", windows)

    def test_unversioned_archive_is_not_a_proven_git_checkout(self):
        with tempfile.TemporaryDirectory() as td:
            self.assertIsNone(GATE.checked_commit(Path(td)))

    def test_plugin_is_resolved_from_project_or_engine_directory(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            engine = root / "UE"
            project = root / "Game"
            plugin = project / "Plugins" / "ExamplePlugin" / "ExamplePlugin.uplugin"
            plugin.parent.mkdir(parents=True)
            plugin.write_text("{}", encoding="utf-8")
            self.assertTrue(GATE.find_plugin(engine, project, "ExamplePlugin"))
            self.assertFalse(GATE.find_plugin(engine, project, "NotInstalled"))

    def test_missing_engine_and_lfs_payloads_fail_closed_without_builds(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            project = root / "Game"
            evidence = root / "Evidence"
            (project / "Source").mkdir(parents=True)
            (project / "Content").mkdir(parents=True)
            (project / "MORABEZA.uproject").write_text(
                json.dumps({"EngineAssociation": "5.8", "Plugins": []}), encoding="utf-8"
            )
            for target in GATE.TARGETS:
                (project / "Source" / (target + ".Target.cs")).write_text("// fixture", encoding="utf-8")
            pointer = b"version https://git-lfs.github.com/spec/v1\noid sha256:" + b"0" * 64 + b"\nsize 5\n"
            for index in range(GATE.EXPECTED_LFS_PACKAGES):
                (project / "Content" / f"Fixture{index:03}.uasset").write_bytes(pointer)
            argv = [
                str(MODULE_PATH), "--project-root", str(project),
                "--candidate-sha", EXAMPLE_SHA, "--evidence-dir", str(evidence), "--execute",
            ]
            with mock.patch.object(sys, "argv", argv), mock.patch.object(
                GATE, "run_source_preflight", return_value=(True, "SOURCE PASS")
            ), mock.patch.object(GATE, "checked_commit", return_value=EXAMPLE_SHA):
                result = GATE.main()
            data = json.loads((evidence / "int02-build-evidence.json").read_text(encoding="utf-8"))
            self.assertEqual(result, 2)
            self.assertEqual(data["status"], "BLOCKED_NO_UNREAL_BUILD")
            self.assertIn("LFS_PAYLOADS_MISSING:293", data["blockers"])
            self.assertIn("UE_ENGINE_ROOT_UNAVAILABLE", data["blockers"])
            self.assertEqual(data["builds"], [])

    def test_incorrect_engine_version_is_blocking(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            engine = root / "UE"
            project = root / "Game"
            evidence = root / "Evidence"
            (engine / "Engine" / "Build" / "BatchFiles" / "Linux").mkdir(parents=True)
            (engine / "Engine" / "Build" / "Build.version").write_text(
                json.dumps({"MajorVersion": 5, "MinorVersion": 7}), encoding="utf-8"
            )
            (engine / "Engine" / "Build" / "BatchFiles" / "Linux" / "Build.sh").write_text(
                "#!/bin/sh\nexit 0\n", encoding="utf-8"
            )
            (project / "Source").mkdir(parents=True)
            (project / "Content").mkdir(parents=True)
            (project / "MORABEZA.uproject").write_text(
                json.dumps({"EngineAssociation": "5.8", "Plugins": []}), encoding="utf-8"
            )
            for target in GATE.TARGETS:
                (project / "Source" / (target + ".Target.cs")).write_text("// fixture", encoding="utf-8")
            for index in range(GATE.EXPECTED_LFS_PACKAGES):
                (project / "Content" / f"Fixture{index:03}.uasset").write_bytes(b"BINARYPAYLOAD")
            argv = [
                str(MODULE_PATH), "--project-root", str(project),
                "--candidate-sha", EXAMPLE_SHA, "--evidence-dir", str(evidence),
                "--engine-root", str(engine),
            ]
            with mock.patch.object(sys, "argv", argv), mock.patch.object(
                GATE, "run_source_preflight", return_value=(True, "SOURCE PASS")
            ), mock.patch.object(GATE, "checked_commit", return_value=EXAMPLE_SHA):
                result = GATE.main()
            data = json.loads((evidence / "int02-build-evidence.json").read_text(encoding="utf-8"))
            self.assertEqual(result, 2)
            self.assertIn("ENGINE_NOT_UE_5_8", data["blockers"])
            self.assertEqual(data["builds"], [])


if __name__ == "__main__":
    unittest.main()
