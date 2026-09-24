#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "MORABEZAInteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FMORABEZAInteractionPromptChanged,
    const FText&,
    Prompt
);

UCLASS(
    ClassGroup=(MORABEZA),
    BlueprintType,
    meta=(BlueprintSpawnableComponent)
)
class MORABEZA_API UMORABEZAInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:

    UMORABEZAInteractionComponent();

    UFUNCTION(BlueprintCallable, Category="Interaction")
    void TryInteract();

    UFUNCTION(BlueprintCallable, Category="Interaction")
    void UpdateInteractionTarget();

    // Server-only entry from the owning AMORABEZACharacter RPC.
    // RequestedTarget is untrusted and is never executed without revalidation.
    void ExecuteServerInteraction(AActor* RequestedTarget);

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category="Interaction"
    )
    float InteractionDistance = 350.0f;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category="Interaction"
    )
    float InteractionTraceDistance = 2000.0f;

    UPROPERTY(
        BlueprintAssignable,
        Category="Interaction"
    )
    FMORABEZAInteractionPromptChanged OnPromptChanged;

protected:

    virtual void BeginPlay() override;

    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction
    ) override;

private:

    TWeakObjectPtr<AActor> CurrentTarget;

    // Server-side request spacing only. This is NOT a durable replay ledger.
    double LastServerInteractionSeconds = -1000.0;

    void SetInteractionTarget(AActor* NewTarget);
};
