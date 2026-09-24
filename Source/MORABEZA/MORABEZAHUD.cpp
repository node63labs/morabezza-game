#include "MORABEZAHUD.h"

#include "MORABEZAInteractionWidget.h"
#include "MORABEZADialogueWidget.h"
#include "MORABEZACharacter.h"
#include "MORABEZAInteractionComponent.h"
#include "Dialogue/MORABEZADialogueComponent.h"

#include "GameFramework/PlayerController.h"

AMORABEZAHUD::AMORABEZAHUD()
{
    // The owning controller may receive its pawn after HUD BeginPlay.
    // Rebinding is a cheap pointer comparison; no widgets are rebuilt per tick.
    PrimaryActorTick.bCanEverTick = true;
}

void AMORABEZAHUD::BeginPlay()
{
    Super::BeginPlay();
    InitializeLocalUI();
    SynchronizeInteractionBinding();
}

void AMORABEZAHUD::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Also covers a HUD that began play before its owning controller existed.
    InitializeLocalUI();
    SynchronizeInteractionBinding();
}

void AMORABEZAHUD::EndPlay(
    const EEndPlayReason::Type EndPlayReason
)
{
    if (UMORABEZAInteractionComponent* Previous =
            BoundInteractionComponent.Get())
    {
        if (bPromptDelegateBound && IsValid(InteractionWidget))
        {
            Previous->OnPromptChanged.RemoveDynamic(
                InteractionWidget,
                &UMORABEZAInteractionWidget::SetInteractionPrompt
            );
        }
    }

    ClearDialogueForPawnChange();
    BoundPawn.Reset();
    BoundInteractionComponent.Reset();
    bHasInteractionBinding = false;
    bPromptDelegateBound = false;

    Super::EndPlay(EndPlayReason);
}

void AMORABEZAHUD::InitializeLocalUI()
{
    if (bLocalUIInitialized)
    {
        return;
    }

    APlayerController* PlayerController = GetOwningPlayerController();
    if (!IsValid(PlayerController) ||
        !PlayerController->IsLocalController())
    {
        // No owner yet; the next HUD tick will retry.
        return;
    }

    // The controller is now available. Create each widget at most once.
    bLocalUIInitialized = true;

    InteractionWidget =
        CreateWidget<UMORABEZAInteractionWidget>(
            PlayerController,
            UMORABEZAInteractionWidget::StaticClass()
        );

    if (IsValid(InteractionWidget))
    {
        InteractionWidget->AddToViewport(100);
        InteractionWidget->SetInteractionPrompt(FText::GetEmpty());
    }
    else
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("MORABEZA HUD: Failed to create interaction widget.")
        );
    }

    DialogueWidget =
        CreateWidget<UMORABEZADialogueWidget>(
            PlayerController,
            UMORABEZADialogueWidget::StaticClass()
        );

    if (IsValid(DialogueWidget))
    {
        DialogueWidget->AddToViewport(1000);
        DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
        DialogueWidget->SetRenderOpacity(1.0f);
    }
    else
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("MORABEZA HUD: Failed to create dialogue widget.")
        );
    }

    FInputModeGameOnly InputMode;
    PlayerController->SetInputMode(InputMode);
    PlayerController->bShowMouseCursor = false;
}

void AMORABEZAHUD::ClearDialogueForPawnChange()
{
    // The last contact may still have delegates even after its dialogue
    // finished and ActiveDialogueComponent was cleared.
    if (UMORABEZADialogueComponent* Previous = LastDialogueComponent.Get())
    {
        Previous->OnDialogueLineChanged.RemoveAll(this);
        Previous->OnDialogueFinished.RemoveAll(this);
    }

    LastDialogueComponent.Reset();
    ActiveDialogueComponent = nullptr;
    bDialogueActive = false;
    CurrentSpeaker = FText::GetEmpty();
    CurrentDialogue = FText::GetEmpty();

    if (IsValid(DialogueWidget))
    {
        DialogueWidget->HideDialogue();
        DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void AMORABEZAHUD::SynchronizeInteractionBinding()
{
    APlayerController* PlayerController = GetOwningPlayerController();

    AMORABEZACharacter* CurrentPawn =
        IsValid(PlayerController) &&
        PlayerController->IsLocalController()
            ? Cast<AMORABEZACharacter>(PlayerController->GetPawn())
            : nullptr;

    UMORABEZAInteractionComponent* DesiredComponent =
        IsValid(CurrentPawn) && CurrentPawn->IsLocallyControlled()
            ? CurrentPawn->FindComponentByClass<
                  UMORABEZAInteractionComponent
              >()
            : nullptr;

    // Stable pawn: bind a widget that became available after the component.
    // Avoid duplicate delegates on every HUD tick.
    if (bHasInteractionBinding &&
        BoundPawn.Get() == CurrentPawn &&
        BoundInteractionComponent.Get() == DesiredComponent &&
        IsValid(DesiredComponent))
    {
        if (!bPromptDelegateBound && IsValid(InteractionWidget))
        {
            DesiredComponent->OnPromptChanged.RemoveDynamic(
                InteractionWidget,
                &UMORABEZAInteractionWidget::SetInteractionPrompt
            );
            DesiredComponent->OnPromptChanged.AddDynamic(
                InteractionWidget,
                &UMORABEZAInteractionWidget::SetInteractionPrompt
            );
            bPromptDelegateBound = true;
            DesiredComponent->UpdateInteractionTarget();
        }
        return;
    }

    if (!bHasInteractionBinding && !IsValid(DesiredComponent))
    {
        return;
    }

    // A different pawn (or no pawn) invalidates all prior UI bindings.
    if (UMORABEZAInteractionComponent* Previous =
            BoundInteractionComponent.Get())
    {
        if (bPromptDelegateBound && IsValid(InteractionWidget))
        {
            Previous->OnPromptChanged.RemoveDynamic(
                InteractionWidget,
                &UMORABEZAInteractionWidget::SetInteractionPrompt
            );
        }
    }

    if (bHasInteractionBinding)
    {
        ClearDialogueForPawnChange();
    }

    BoundPawn.Reset();
    BoundInteractionComponent.Reset();
    bHasInteractionBinding = false;
    bPromptDelegateBound = false;

    if (IsValid(InteractionWidget))
    {
        InteractionWidget->SetInteractionPrompt(FText::GetEmpty());
    }

    if (!IsValid(DesiredComponent))
    {
        return;
    }

    BoundPawn = CurrentPawn;
    BoundInteractionComponent = DesiredComponent;
    bHasInteractionBinding = true;

    if (IsValid(InteractionWidget))
    {
        DesiredComponent->OnPromptChanged.RemoveDynamic(
            InteractionWidget,
            &UMORABEZAInteractionWidget::SetInteractionPrompt
        );
        DesiredComponent->OnPromptChanged.AddDynamic(
            InteractionWidget,
            &UMORABEZAInteractionWidget::SetInteractionPrompt
        );
        bPromptDelegateBound = true;

        // A prompt may have been detected before the HUD bound its delegate.
        DesiredComponent->UpdateInteractionTarget();
    }
}

void AMORABEZAHUD::OpenDialogue(
    const FText& SpeakerName,
    const FText& DialogueText
)
{
    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA DIALOGUE: OpenDialogue() called."
        )
    );

    if (!DialogueWidget)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "MORABEZA DIALOGUE: DialogueWidget unavailable."
            )
        );

        return;
    }

    /*
     * ============================================================
     * STORE CURRENT LINE
     * ============================================================
     */

    CurrentSpeaker = SpeakerName;
    CurrentDialogue = DialogueText;

    /*
     * ============================================================
     * ACTIVATE DIALOGUE
     * ============================================================
     */

    bDialogueActive = true;

    /*
     * ============================================================
     * SHOW DIALOGUE
     * ============================================================
     */

    DialogueWidget->SetVisibility(
        ESlateVisibility::Visible
    );

    DialogueWidget->SetRenderOpacity(1.0f);

    DialogueWidget->ShowDialogue(
        CurrentSpeaker,
        CurrentDialogue
    );

    /*
     * ============================================================
     * HIDE INTERACTION PROMPT
     * ============================================================
     */

    if (InteractionWidget)
    {
        InteractionWidget->SetVisibility(
            ESlateVisibility::Collapsed
        );
    }

    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA DIALOGUE: OPEN - Speaker=%s Text=%s"
        ),
        *SpeakerName.ToString(),
        *DialogueText.ToString()
    );
}

void AMORABEZAHUD::SetActiveDialogueComponent(
    UMORABEZADialogueComponent* DialogueComponent
)
{
    if (UMORABEZADialogueComponent* Previous = LastDialogueComponent.Get())
    {
        if (Previous != DialogueComponent)
        {
            Previous->OnDialogueLineChanged.RemoveAll(this);
            Previous->OnDialogueFinished.RemoveAll(this);
        }
    }

    LastDialogueComponent = DialogueComponent;
    ActiveDialogueComponent = DialogueComponent;
    bDialogueActive = false;

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("MORABEZA HUD: Active dialogue component assigned: %s"),
        DialogueComponent
            ? *DialogueComponent->GetName()
            : TEXT("NONE")
    );
}

void AMORABEZAHUD::AdvanceDialogue()
{
    if (!bDialogueActive)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT(
                "MORABEZA HUD: AdvanceDialogue ignored - no active dialogue."
            )
        );

        return;
    }

    if (ActiveDialogueComponent)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT(
                "MORABEZA HUD: Advancing active dialogue."
            )
        );

        ActiveDialogueComponent->AdvanceDialogue();

        return;
    }

    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA HUD: No active dialogue component."
        )
    );
}

void AMORABEZAHUD::HandleDialogueLineChanged(
    const FText& Speaker,
    const FText& Text
)
{
    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA HUD: Dialogue line received."
        )
    );

    OpenDialogue(
        Speaker,
        Text
    );
}

void AMORABEZAHUD::HandleDialogueFinished()
{
    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA DIALOGUE: Finished."
        )
    );

    /*
     * ============================================================
     * RESET DIALOGUE STATE
     * ============================================================
     */

    bDialogueActive = false;

    ActiveDialogueComponent = nullptr;

    /*
     * ============================================================
     * HIDE DIALOGUE
     * ============================================================
     */

    if (DialogueWidget)
    {
        DialogueWidget->HideDialogue();

        DialogueWidget->SetVisibility(
            ESlateVisibility::Collapsed
        );

        DialogueWidget->SetRenderOpacity(1.0f);
    }

    /*
     * ============================================================
     * RESTORE INTERACTION PROMPT
     * ============================================================
     */

    if (InteractionWidget)
    {
        InteractionWidget->SetVisibility(
            ESlateVisibility::Visible
        );
    }

    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA DIALOGUE: CLOSED."
        )
    );
}

bool AMORABEZAHUD::IsDialogueActive() const
{
    return bDialogueActive;
}