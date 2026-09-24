"""S0-W1b source-contract regression checks (offline, no Unreal or GitHub Actions).

These checks verify that required guardrails remain present in the source.
They do not compile Unreal, execute RPCs, prove hit geometry, or replace
two-client negative tests.
"""

from pathlib import Path
import unittest


ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / "Source" / "MORABEZA"


def source(name: str) -> str:
    return (SOURCE / name).read_text(encoding="utf-8")


class ServerInteractionSourceContract(unittest.TestCase):
    def test_owned_pawn_declares_server_and_client_rpcs(self) -> None:
        header = source("MORABEZACharacter.h")
        implementation = source("MORABEZACharacter.cpp")
        self.assertIn("UFUNCTION(Server, Reliable)", header)
        self.assertIn("ServerTryInteract(AActor* RequestedTarget)", header)
        self.assertIn("UFUNCTION(Client, Reliable)", header)
        self.assertIn("ClientPresentValidatedContact(AMORABEZAContactActor* Contact)", header)
        self.assertIn("ServerTryInteract_Implementation(", implementation)
        self.assertIn("ClientPresentValidatedContact_Implementation(", implementation)
        self.assertIn("InteractionComponent->ExecuteServerInteraction(RequestedTarget)", implementation)

    def test_local_input_never_executes_interface(self) -> None:
        implementation = source("MORABEZAInteractionComponent.cpp")
        start = implementation.index("void UMORABEZAInteractionComponent::TryInteract()")
        end = implementation.index("void UMORABEZAInteractionComponent::ExecuteServerInteraction(", start)
        local = implementation[start:end]
        self.assertNotIn("Execute_Interact(", local)
        self.assertIn("Character->IsLocallyControlled()", local)
        self.assertIn("Character->ServerTryInteract(RequestedTarget)", local)

    def test_server_rechecks_pawn_owner_and_target(self) -> None:
        implementation = source("MORABEZAInteractionComponent.cpp")
        start = implementation.index("void UMORABEZAInteractionComponent::ExecuteServerInteraction(")
        server = implementation[start:]
        for guard in (
            "Character->HasAuthority()",
            "PlayerController->GetPawn() != Character",
            "RequestedTarget->GetWorld() != World",
            "UMORABEZAInteractable::StaticClass()",
            'Contact->MissionId != FName(TEXT("TEST_INTERACTION"))',
            "Contact->GetOwner() != PlayerController",
            "World->GetNetMode() != NM_Standalone",
            "Now - LastServerInteractionSeconds < 0.35",
            "FMath::Min(InteractionDistance, 350.0f)",
            "SweepHit.GetActor() != RequestedTarget",
            "LineHit.GetActor() != RequestedTarget",
        ):
            with self.subTest(guard=guard):
                self.assertIn(guard, server)

        # All authoritative guards must precede the effect.
        effect = server.index("IMORABEZAInteractable::Execute_Interact(")
        for guard in (
            "Character->HasAuthority()",
            "Contact->GetOwner() != PlayerController",
            "SweepHit.GetActor() != RequestedTarget",
        ):
            self.assertLess(server.index(guard), effect)

    def test_contact_effect_routes_only_to_owning_client(self) -> None:
        implementation = source("MORABEZAContactActor.cpp")
        start = implementation.index("void AMORABEZAContactActor::Interact_Implementation(")
        end = implementation.index("void AMORABEZAContactActor::PresentDialogueToLocalPlayer(", start)
        authoritative = implementation[start:end]
        self.assertIn("!HasAuthority()", authoritative)
        self.assertIn("GetOwner() != PlayerController", authoritative)
        self.assertIn("PlayerController->IsLocalController()", authoritative)
        self.assertIn("PlayerCharacter->ClientPresentValidatedContact(this)", authoritative)
        self.assertNotIn("GetFirstPlayerController()", implementation)
        presentation = implementation[end:]
        self.assertNotIn("Execute_Interact(", presentation)
        self.assertIn("!PlayerController->IsLocalController()", presentation)

    def test_network_prompt_restricts_to_owner_scoped_contact(self) -> None:
        implementation = source("MORABEZAInteractionComponent.cpp")
        start = implementation.index(
            "void UMORABEZAInteractionComponent::UpdateInteractionTarget()"
        )
        end = implementation.index(
            "void UMORABEZAInteractionComponent::TryInteract()", start
        )
        prompt = implementation[start:end]
        for guard in (
            "Character->IsLocallyControlled()",
            "World->GetNetMode() != NM_Standalone",
            'Contact->MissionId != FName(TEXT("TEST_INTERACTION"))',
            "Contact->GetOwner() != Character->GetController()",
            "Contact->GetIsReplicated()",
        ):
            with self.subTest(guard=guard):
                self.assertIn(guard, prompt)

    def test_first_player_fallback_is_absent_from_contact_and_game_mode(self) -> None:
        for name in ("MORABEZAContactActor.cpp", "MORABEZAGameMode.cpp"):
            with self.subTest(file=name):
                self.assertNotIn("GetFirstPlayerController()", source(name))

    def test_server_effect_is_not_accessible_from_client_input(self) -> None:
        implementation = source("MORABEZAInteractionComponent.cpp")
        self.assertEqual(
            implementation.count("IMORABEZAInteractable::Execute_Interact("),
            1,
        )
        effect = implementation.index(
            "IMORABEZAInteractable::Execute_Interact("
        )
        authoritative_method = implementation.index(
            "void UMORABEZAInteractionComponent::ExecuteServerInteraction("
        )
        self.assertGreater(effect, authoritative_method)

    def test_local_dialogue_presentation_does_not_grant_rewards(self) -> None:
        implementation = source("MORABEZAContactActor.cpp")
        start = implementation.index(
            "void AMORABEZAContactActor::PresentDialogueToLocalPlayer("
        )
        presentation = implementation[start:]
        self.assertIn("!PlayerController->IsLocalController()", presentation)
        self.assertIn("GetOwner() != PlayerController", presentation)
        for forbidden in (
            "Execute_Interact(",
            "AddMoney(",
            "SpendMoney(",
            "AddReputation(",
            "GrantItem(",
            "CompleteMission(",
        ):
            with self.subTest(forbidden=forbidden):
                self.assertNotIn(forbidden, presentation)


if __name__ == "__main__":
    unittest.main()
