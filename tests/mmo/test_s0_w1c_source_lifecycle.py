"""S0-W1c source-only lifecycle regression checks (no Unreal/Actions required).

These tests inspect required source guardrails, not actual Unreal possession,
network replication, or generated C++ compilation.
"""
from pathlib import Path
import unittest

SOURCE = Path(__file__).resolve().parents[2] / "Source" / "MORABEZA"


def read(name: str) -> str:
    return (SOURCE / name).read_text(encoding="utf-8")


def method(text: str, signature: str, next_signature: str) -> str:
    start = text.index(signature)
    end = text.index(next_signature, start + len(signature))
    return text[start:end]


class PossessionSafeInputAndHUD(unittest.TestCase):
    def test_input_retries_after_client_restart_and_controller_replication(self) -> None:
        header = read("MORABEZACharacter.h")
        implementation = read("MORABEZACharacter.cpp")
        self.assertIn("virtual void PawnClientRestart() override;", header)
        self.assertIn("virtual void OnRep_Controller() override;", header)
        restart = method(
            implementation,
            "void AMORABEZACharacter::PawnClientRestart()",
            "void AMORABEZACharacter::OnRep_Controller()",
        )
        on_rep = method(
            implementation,
            "void AMORABEZACharacter::OnRep_Controller()",
            "void AMORABEZACharacter::EndPlay(",
        )
        self.assertIn("Super::PawnClientRestart();", restart)
        self.assertIn("RefreshLocalInputMapping();", restart)
        self.assertIn("Super::OnRep_Controller();", on_rep)
        self.assertIn("RefreshLocalInputMapping();", on_rep)

    def test_local_mapping_preserves_other_contexts_and_is_idempotent(self) -> None:
        implementation = read("MORABEZACharacter.cpp")
        self.assertNotIn("ClearAllMappings()", implementation)
        refresh = method(
            implementation,
            "void AMORABEZACharacter::RefreshLocalInputMapping()",
            "void AMORABEZACharacter::Tick(",
        )
        self.assertIn("IsLocallyControlled()", refresh)
        self.assertIn("GetLocalPlayer()", refresh)
        self.assertIn("DesiredSubsystem->HasMappingContext(InputMappingContext)", refresh)
        self.assertIn("DesiredSubsystem->AddMappingContext(InputMappingContext, 0)", refresh)
        self.assertIn("RegisteredInputSubsystem = DesiredSubsystem", refresh)
        self.assertLess(
            refresh.index("if (!DesiredSubsystem->HasMappingContext("),
            refresh.index("DesiredSubsystem->AddMappingContext("),
        )

    def test_old_pawn_context_is_removed_only_from_its_subsystem(self) -> None:
        implementation = read("MORABEZACharacter.cpp")
        unregister = method(
            implementation,
            "void AMORABEZACharacter::UnregisterLocalInputMapping()",
            "void AMORABEZACharacter::RefreshLocalInputMapping()",
        )
        self.assertIn("Subsystem->RemoveMappingContext(InputMappingContext)", unregister)
        self.assertIn("RegisteredInputSubsystem.Reset()", unregister)
        self.assertNotIn("ClearAllMappings", unregister)
        end_play = method(
            implementation,
            "void AMORABEZACharacter::EndPlay(",
            "void AMORABEZACharacter::UnregisterLocalInputMapping()",
        )
        self.assertIn("UnregisterLocalInputMapping();", end_play)

    def test_input_binding_is_reconciled_after_setup(self) -> None:
        implementation = read("MORABEZACharacter.cpp")
        setup = method(
            implementation,
            "void AMORABEZACharacter::SetupPlayerInputComponent(",
            "void AMORABEZACharacter::MoveForward(",
        )
        self.assertIn("RefreshLocalInputMapping();", setup)
        self.assertIn("&AMORABEZACharacter::HandleInteract", setup)
        self.assertIn("&AMORABEZACharacter::MoveForward", setup)

    def test_hud_setup_retries_when_owning_controller_is_late(self) -> None:
        header = read("MORABEZAHUD.h")
        implementation = read("MORABEZAHUD.cpp")
        self.assertIn("virtual void Tick(float DeltaTime) override;", header)
        self.assertIn("PrimaryActorTick.bCanEverTick = true;", implementation)
        begin = method(
            implementation,
            "void AMORABEZAHUD::BeginPlay()",
            "void AMORABEZAHUD::Tick(",
        )
        tick = method(
            implementation,
            "void AMORABEZAHUD::Tick(",
            "void AMORABEZAHUD::EndPlay(",
        )
        for piece in (begin, tick):
            self.assertIn("InitializeLocalUI();", piece)
            self.assertIn("SynchronizeInteractionBinding();", piece)
        init = method(
            implementation,
            "void AMORABEZAHUD::InitializeLocalUI()",
            "void AMORABEZAHUD::ClearDialogueForPawnChange()",
        )
        self.assertIn("PlayerController->IsLocalController()", init)
        self.assertIn("if (bLocalUIInitialized)", init)
        self.assertIn("bLocalUIInitialized = true;", init)

    def test_hud_rebind_detaches_old_prompt_and_prevents_duplicates(self) -> None:
        implementation = read("MORABEZAHUD.cpp")
        sync = method(
            implementation,
            "void AMORABEZAHUD::SynchronizeInteractionBinding()",
            "void AMORABEZAHUD::OpenDialogue(",
        )
        self.assertIn("BoundPawn.Get() == CurrentPawn", sync)
        self.assertIn("BoundInteractionComponent.Get() == DesiredComponent", sync)
        self.assertIn("bPromptDelegateBound", sync)
        self.assertIn("Previous->OnPromptChanged.RemoveDynamic(", sync)
        self.assertIn("DesiredComponent->OnPromptChanged.AddDynamic(", sync)
        self.assertIn("DesiredComponent->UpdateInteractionTarget();", sync)
        self.assertIn("BoundPawn.Reset();", sync)
        self.assertIn("InteractionWidget->SetInteractionPrompt(FText::GetEmpty())", sync)

    def test_pawn_change_clears_old_dialogue_subscriptions_and_ui(self) -> None:
        implementation = read("MORABEZAHUD.cpp")
        cleanup = method(
            implementation,
            "void AMORABEZAHUD::ClearDialogueForPawnChange()",
            "void AMORABEZAHUD::SynchronizeInteractionBinding()",
        )
        self.assertIn("OnDialogueLineChanged.RemoveAll(this)", cleanup)
        self.assertIn("OnDialogueFinished.RemoveAll(this)", cleanup)
        self.assertIn("ActiveDialogueComponent = nullptr", cleanup)
        self.assertIn("bDialogueActive = false", cleanup)
        self.assertIn("DialogueWidget->HideDialogue();", cleanup)

    def test_finished_dialogue_still_unbinds_on_next_pawn(self) -> None:
        header = read("MORABEZAHUD.h")
        implementation = read("MORABEZAHUD.cpp")
        self.assertIn(
            "TWeakObjectPtr<UMORABEZADialogueComponent> LastDialogueComponent",
            header,
        )
        setter = method(
            implementation,
            "void AMORABEZAHUD::SetActiveDialogueComponent(",
            "void AMORABEZAHUD::AdvanceDialogue()",
        )
        self.assertIn("LastDialogueComponent = DialogueComponent", setter)
        cleanup = method(
            implementation,
            "void AMORABEZAHUD::ClearDialogueForPawnChange()",
            "void AMORABEZAHUD::SynchronizeInteractionBinding()",
        )
        self.assertIn("LastDialogueComponent.Get()", cleanup)
        self.assertIn("LastDialogueComponent.Reset()", cleanup)

    def test_hud_endplay_detaches_prompt_and_dialogue(self) -> None:
        implementation = read("MORABEZAHUD.cpp")
        endplay = method(
            implementation,
            "void AMORABEZAHUD::EndPlay(",
            "void AMORABEZAHUD::InitializeLocalUI()",
        )
        self.assertIn("Previous->OnPromptChanged.RemoveDynamic(", endplay)
        self.assertIn("ClearDialogueForPawnChange();", endplay)
        self.assertIn("Super::EndPlay(EndPlayReason);", endplay)


if __name__ == "__main__":
    unittest.main()
