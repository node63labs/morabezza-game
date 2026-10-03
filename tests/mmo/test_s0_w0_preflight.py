"""S0-W0 source-only preflight tests. No Unreal executable is run."""

from __future__ import annotations

import os
from pathlib import Path
import subprocess
import unittest


ROOT = Path(__file__).resolve().parents[2]
HARNESS = ROOT / "scripts" / "mmo" / "s0_two_client_smoke.sh"
SERVER_TARGET = ROOT / "Source" / "MORABEZAServer.Target.cs"
GAME_TARGET = ROOT / "Source" / "MORABEZA.Target.cs"


class ServerHarnessPreflightTests(unittest.TestCase):
    def setUp(self) -> None:
        self.env = dict(os.environ)
        self.env.update(
            MORABEZA_SERVER_BIN="/bin/echo",
            MORABEZA_CLIENT_BIN="/bin/echo",
            MORABEZA_TEST_MAP="/Game/Dev/MMO_S0_TestZone",
            MORABEZA_TEST_PORT="7777",
            MORABEZA_SMOKE_WAIT="3",
        )

    def call(self, *args: str, env=None) -> subprocess.CompletedProcess[str]:
        return subprocess.run(
            ["bash", str(HARNESS), *args],
            text=True,
            capture_output=True,
            env=env if env is not None else self.env,
            cwd=ROOT,
            check=False,
        )

    def test_valid_plan_is_non_executing_and_proposes_two_clients(self) -> None:
        result = self.call("--plan")
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("S0-W0 PLAN PASS", result.stdout)
        self.assertIn("server:", result.stdout)
        self.assertIn("client A:", result.stdout)
        self.assertIn("client B:", result.stdout)
        self.assertIn("/Game/Dev/MMO_S0_TestZone", result.stdout)
        self.assertIn("127.0.0.1:7777", result.stdout)

    def test_refuse_missing_binaries(self) -> None:
        env = dict(self.env)
        env.pop("MORABEZA_SERVER_BIN")
        result = self.call("--plan", env=env)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("MORABEZA_SERVER_BIN", result.stderr)

    def test_refuse_candidate_production_map(self) -> None:
        for path in ("/Game/dskjnfkljsdnf", "/Game/Untitled", "/Game/Dev/../dskjnfkljsdnf"):
            with self.subTest(path=path):
                env = dict(self.env)
                env["MORABEZA_TEST_MAP"] = path
                result = self.call("--plan", env=env)
                self.assertNotEqual(result.returncode, 0)
                self.assertIn("MORABEZA_TEST_MAP", result.stderr)

    def test_refuse_invalid_port(self) -> None:
        for port in ("1023", "65536", "7777;touch /tmp/unsafe", "0"):
            with self.subTest(port=port):
                env = dict(self.env)
                env["MORABEZA_TEST_PORT"] = port
                result = self.call("--plan", env=env)
                self.assertNotEqual(result.returncode, 0)
                self.assertIn("MORABEZA_TEST_PORT", result.stderr)

    def test_refuse_unbounded_observation_period(self) -> None:
        env = dict(self.env)
        env["MORABEZA_SMOKE_WAIT"] = "120"
        result = self.call("--plan", env=env)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("MORABEZA_SMOKE_WAIT", result.stderr)

    def test_unreal_server_target_mirrors_existing_build_authority(self) -> None:
        source = SERVER_TARGET.read_text(encoding="utf-8")
        game = GAME_TARGET.read_text(encoding="utf-8")
        self.assertIn("public class MORABEZAServerTarget : TargetRules", source)
        self.assertIn("Type = TargetType.Server;", source)
        self.assertIn('ExtraModuleNames.Add("MORABEZA");', source)
        for expected in (
            "DefaultBuildSettings = BuildSettingsVersion.Latest;",
            "IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;",
        ):
            self.assertIn(expected, source)
            self.assertIn(expected, game)


if __name__ == "__main__":
    unittest.main()
