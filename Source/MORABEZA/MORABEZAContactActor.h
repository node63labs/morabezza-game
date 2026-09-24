#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MORABEZAInteractable.h"
#include "MORABEZAContactActor.generated.h"

class UStaticMeshComponent;
class UMORABEZADialogueComponent;

UCLASS(Blueprintable)
class MORABEZA_API AMORABEZAContactActor
    : public AActor
    , public IMORABEZAInteractable
{
    GENERATED_BODY()

public:

    AMORABEZAContactActor();

    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps
    ) const override;

    virtual void Interact_Implementation(
        AActor* Interactor
    ) override;

    virtual FText GetInteractionPrompt_Implementation()
        const override;

    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly,
        Category="Contact"
    )
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly,
        Category="Dialogue"
    )
    TObjectPtr<UMORABEZADialogueComponent> DialogueComponent;

    UPROPERTY(
        Replicated,
        EditAnywhere,
        BlueprintReadWrite,
        Category="Mission"
    )
    FName MissionId = TEXT("VOLTA_INTRO");

    UPROPERTY(
        Replicated,
        EditAnywhere,
        BlueprintReadWrite,
        Category="Mission"
    )
    FText ContactName =
        FText::FromString(TEXT("Contact"));
};
