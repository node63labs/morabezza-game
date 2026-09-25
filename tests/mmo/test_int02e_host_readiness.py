"""INT-02E fixture tests. No Unreal process, download or remote Git operation."""
from __future__ import annotations

import importlib.util
import json
from pathlib import Path
import tempfile
import unittest
from unittest import mock

MODULE = Path(__file__).resolve().parents[2] / "scripts" / "mmo" / "int02e_host_readiness.py"
SPEC = importlib.util.spec_from_file_location("int02e_host_readiness", MODULE)
assert SPEC and SPEC.loader
GATE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(GATE)
SHA = "a" * 40


class Int02EHostReadinessTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.project = self.root / "project"
        self.engine = self.root / "engine"
        (self.project / "Content").mkdir(parents=True)
        (self.project / "Source").mkdir()
        (self.project / "MORABEZA.uproject").write_text(
            json.dumps({"EngineAssociation": "5.8", "Plugins": [
                {"Name": "EnhancedInput", "Enabled": True},
                {"Name": "MoverExamples", "Enabled": True}]}), encoding="utf-8")
        for name in GATE.TARGETS:
            (self.project / "Source" / name).write_text("// source", encoding="utf-8")

    def assets(self, hydrated=True):
        content = b"BINARY\0" if hydrated else GATE.POINTER + b"\noid sha256:" + b"0" * 64
        for index in range(GATE.EXPECTED_ASSETS):
            (self.project / "Content" / f"P{index:03}.uasset").write_bytes(content)

    def engine_fixture(self, version=(5, 8), missing_plugin=None):
        root = self.engine / "Engine"
        (root / "Build" / "BatchFiles" / "Linux").mkdir(parents=True)
        (root / "Build" / "Build.version").write_text(
            json.dumps({"MajorVersion": version[0], "MinorVersion": version[1]}),
            encoding="utf-8")
        (root / "Build" / "BatchFiles" / "Linux" / "Build.sh").write_text(
            "#!/bin/sh\nexit 0\n", encoding="utf-8")
        (root / "Source" / "Runtime").mkdir(parents=True)
        (root / "Source" / "Programs" / "UnrealBuildTool").mkdir(parents=True)
        for name in GATE.REQUIRED_PLUGINS:
            if name == missing_plugin:
                continue
            plugin = root / "Plugins" / "Experimental" / name / (name + ".uplugin")
            plugin.parent.mkdir(parents=True)
            plugin.write_text("{}", encoding="utf-8")

    def fake_git_checkout(self, actual=SHA, lfs_fsck_exit=0):
        (self.project / ".git").mkdir()
        calls = []

        def handler(args, cwd, timeout=20):
            calls.append(args[1:])
            if args[1:] == ["rev-parse", "HEAD"]:
                return 0, actual
            if args[1:] == ["status", "--porcelain", "--untracked-files=no"]:
                return 0, ""
            if args[1:] == ["lfs", "version"]:
                return 0, "git-lfs/3.6.0"
            if args[1:] == ["lfs", "fsck", "--objects", "--dry-run"]:
                return lfs_fsck_exit, ""
            raise AssertionError("unexpected shell command " + repr(args))
        return handler, calls

    def test_archive_missing_git_engine_and_lfs_is_blocked(self):
        self.assets(hydrated=False)
        report = GATE.inspect(self.project, None, SHA)
        self.assertEqual(report["status"], "BLOCKED")
        self.assertIn("EXACT_GIT_CHECKOUT_REQUIRED", report["blockers"])
        self.assertIn("LFS_PAYLOADS_NOT_HYDRATED:293", report["blockers"])
        self.assertIn("UE58_ENGINE_ROOT_MISSING", report["blockers"])
        self.assertEqual(report["builds_executed"], 0)

    def test_host_can_be_eligible_but_never_implicitly_approved(self):
        self.assets()
        self.engine_fixture()
        git, calls = self.fake_git_checkout()
        with mock.patch.object(GATE, "command", side_effect=git), mock.patch.object(
            GATE.shutil, "which", return_value="/usr/bin/git"
        ), mock.patch.object(GATE.platform, "system", return_value="Linux"):
            report = GATE.inspect(self.project, self.engine, SHA)
        self.assertEqual(report["status"], "HOST_PREFLIGHT_ELIGIBLE_REVIEW_REQUIRED")
        self.assertEqual(report["blockers"], [])
        self.assertEqual(report["lfs_pointer_placeholders"], 0)
        self.assertIn("MOVER_PLUGIN_NOT_EXPLICITLY_ENABLED_VERIFY_EXAMPLES_DEPENDENCY",
                      report["manual_review_required"])
        self.assertFalse(report["production_or_main_authorized"])
        self.assertEqual(report["builds_executed"], 0)
        self.assertEqual(calls, [["rev-parse", "HEAD"],
                                 ["status", "--porcelain", "--untracked-files=no"],
                                 ["lfs", "version"]])

    def test_wrong_pinned_sha_blocks_even_when_other_prerequisites_present(self):
        self.assets()
        self.engine_fixture()
        git, _ = self.fake_git_checkout(actual="b" * 40)
        with mock.patch.object(GATE, "command", side_effect=git), mock.patch.object(
            GATE.shutil, "which", return_value="/usr/bin/git"
        ), mock.patch.object(GATE.platform, "system", return_value="Linux"):
            report = GATE.inspect(self.project, self.engine, SHA)
        self.assertIn("PINNED_GIT_HEAD_MISMATCH", report["blockers"])

    def test_missing_mover_examples_plugin_blocks(self):
        self.assets()
        self.engine_fixture(missing_plugin="MoverExamples")
        git, _ = self.fake_git_checkout()
        with mock.patch.object(GATE, "command", side_effect=git), mock.patch.object(
            GATE.shutil, "which", return_value="/usr/bin/git"
        ), mock.patch.object(GATE.platform, "system", return_value="Linux"):
            report = GATE.inspect(self.project, self.engine, SHA)
        self.assertIn("REQUIRED_PLUGIN_DESCRIPTOR_MISSING:MoverExamples", report["blockers"])

    def test_wrong_engine_minor_version_blocks(self):
        self.assets()
        self.engine_fixture(version=(5, 7))
        git, _ = self.fake_git_checkout()
        with mock.patch.object(GATE, "command", side_effect=git), mock.patch.object(
            GATE.shutil, "which", return_value="/usr/bin/git"
        ), mock.patch.object(GATE.platform, "system", return_value="Linux"):
            report = GATE.inspect(self.project, self.engine, SHA)
        self.assertIn("ENGINE_VERSION_NOT_5_8", report["blockers"])

    def test_lfs_fsck_failure_blocks_only_when_requested(self):
        self.assets()
        self.engine_fixture()
        git, calls = self.fake_git_checkout(lfs_fsck_exit=1)
        with mock.patch.object(GATE, "command", side_effect=git), mock.patch.object(
            GATE.shutil, "which", return_value="/usr/bin/git"
        ), mock.patch.object(GATE.platform, "system", return_value="Linux"):
            report = GATE.inspect(self.project, self.engine, SHA, verify_lfs_objects=True)
        self.assertIn("GIT_LFS_OBJECT_FSCK_FAILED", report["blockers"])
        self.assertEqual(calls[-1], ["lfs", "fsck", "--objects", "--dry-run"])

    def test_missing_server_target_blocks(self):
        self.assets()
        self.engine_fixture()
        (self.project / "Source" / "MORABEZAServer.Target.cs").unlink()
        git, _ = self.fake_git_checkout()
        with mock.patch.object(GATE, "command", side_effect=git), mock.patch.object(
            GATE.shutil, "which", return_value="/usr/bin/git"
        ), mock.patch.object(GATE.platform, "system", return_value="Linux"):
            report = GATE.inspect(self.project, self.engine, SHA)
        self.assertIn("TARGET_MISSING:MORABEZAServer.Target.cs", report["blockers"])


if __name__ == "__main__":
    unittest.main()
