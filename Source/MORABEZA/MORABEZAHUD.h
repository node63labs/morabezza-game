#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MORABEZADialogueWidget.h"
#include "MORABEZAHUD.generated.h"

class UMORABEZAInteractionWidget;
class UMORABEZADialogueWidget;
class UMORABEZADialogueComponent;
class UMORABEZAInteractionComponent;
class AMORABEZACharacter;

UCLASS()
class MORABEZA_API AMORABEZAHUD : public AHUD
{
    GENERATED_BODY()

public:

    AMORABEZAHUD();

    void OpenDialogue(
        const FText& SpeakerName,
        const FText& DialogueText
    );

    void AdvanceDialogue();

    bool IsDialogueActive() const;

    void SetActiveDialogueComponent(
        UMORABEZADialogueComponent* DialogueComponent
    );

    UFUNCTION()
    void HandleDialogueLineChanged(
        const FText& Speaker,
        const FText& Text
    );

    UFUNCTION()
    void HandleDialogueFinished();

protected:

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

    // BeginPlay may precede a valid owning controller or possessed pawn.
    // Retry UI creation and reconcile only when the owned pawn changes.
    void InitializeLocalUI();
    void SynchronizeInteractionBinding();
    void ClearDialogueForPawnChange();

    TWeakObjectPtr<AMORABEZACharacter> BoundPawn;
    TWeakObjectPtr<UMORABEZAInteractionComponent> BoundInteractionComponent;

    bool bLocalUIInitialized = false;
    bool bHasInteractionBinding = false;
    bool bPromptDelegateBound = false;

    UPROPERTY()
    TObjectPtr<UMORABEZAInteractionWidget> InteractionWidget;

    UPROPERTY()
    TObjectPtr<UMORABEZADialogueWidget> DialogueWidget;

    UPROPERTY()
    TObjectPtr<UMORABEZADialogueComponent> ActiveDialogueComponent;

    FText CurrentSpeaker;

    FText CurrentDialogue;

    bool bDialogueActive = false;
};
