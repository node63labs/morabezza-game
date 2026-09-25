"""INT-02C source-only pre-build regression checks.

Checks narrow, reproducible source contract invariants. This suite does NOT
invoke UnrealBuildTool, Unreal Header Tool, a C++ compiler or an RPC runtime.
"""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / "Source"
MODULE = SOURCE / "MORABEZA"


class Int02CPrebuildSourceChecks(unittest.TestCase):
    def test_runtime_commandlet_avoids_editor_only_actor_labels(self) -> None:
        implementation = (MODULE / "LandscapeDiagnosticCommandlet.cpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("GetActorLabel(", implementation)
        self.assertIn("const FString ProxyName = Proxy->GetName();", implementation)

    def test_landscape_commandlet_header_is_independent_of_heavy_landscape_headers(self) -> None:
        header = (MODULE / "LandscapeDiagnosticCommandlet.h").read_text(
            encoding="utf-8"
        )
        for declaration in (
            "class UWorld;",
            "class ALandscapeStreamingProxy;",
            "class ULandscapeComponent;",
        ):
            with self.subTest(declaration=declaration):
                self.assertIn(declaration, header)
        self.assertNotIn('#include "LandscapeProxy.h"', header)
        self.assertNotIn('#include "LandscapeComponent.h"', header)
        self.assertIn('#include "LandscapeDiagnosticCommandlet.generated.h"', header)
        implementation = (MODULE / "LandscapeDiagnosticCommandlet.cpp").read_text(
            encoding="utf-8"
        )
        self.assertIn('#include "LandscapeStreamingProxy.h"', implementation)
        self.assertIn('#include "LandscapeComponent.h"', implementation)

    def test_character_includes_localplayer_without_transitive_dependency(self) -> None:
        implementation = (MODULE / "MORABEZACharacter.cpp").read_text(
            encoding="utf-8"
        )
        self.assertIn('#include "Engine/LocalPlayer.h"', implementation)
        self.assertIn("PlayerController->GetLocalPlayer()", implementation)
        self.assertIn("LocalPlayer->GetSubsystem<", implementation)

    def test_three_build_targets_reference_the_same_runtime_module(self) -> None:
        for target, expected_type in (
            ("MORABEZA.Target.cs", "Game"),
            ("MORABEZAEditor.Target.cs", "Editor"),
            ("MORABEZAServer.Target.cs", "Server"),
        ):
            with self.subTest(target=target):
                contents = (SOURCE / target).read_text(encoding="utf-8")
                self.assertIn("Type = TargetType." + expected_type + ";", contents)
                self.assertIn('ExtraModuleNames.Add("MORABEZA");', contents)
                self.assertIn("EngineIncludeOrderVersion.Unreal5_8", contents)

    def test_reflected_headers_have_generated_header_as_last_include(self) -> None:
        headers = list(MODULE.rglob("*.h"))
        self.assertGreaterEqual(len(headers), 10)
        for header in headers:
            contents = header.read_text(encoding="utf-8")
            if "UCLASS(" not in contents and "UINTERFACE(" not in contents:
                continue
            includes = [
                line.strip() for line in contents.splitlines()
                if line.lstrip().startswith("#include ")
            ]
            with self.subTest(header=str(header.relative_to(ROOT))):
                self.assertTrue(includes)
                self.assertEqual(
                    includes[-1], f'#include "{header.stem}.generated.h"'
                )

    def test_no_runtime_cpp_uses_editor_only_actor_label(self) -> None:
        for implementation in MODULE.rglob("*.cpp"):
            with self.subTest(file=str(implementation.relative_to(ROOT))):
                self.assertNotIn(
                    "GetActorLabel(", implementation.read_text(encoding="utf-8")
                )

    def test_server_rpc_and_owner_guard_are_preserved(self) -> None:
        character_header = (MODULE / "MORABEZACharacter.h").read_text(
            encoding="utf-8"
        )
        interaction = (MODULE / "MORABEZAInteractionComponent.cpp").read_text(
            encoding="utf-8"
        )
        self.assertIn("UFUNCTION(Server, Reliable)", character_header)
        self.assertIn("void ServerTryInteract(AActor* RequestedTarget);", character_header)
        self.assertIn("Character->HasAuthority()", interaction)
        self.assertIn("Contact->GetOwner() != PlayerController", interaction)


if __name__ == "__main__":
    unittest.main()
