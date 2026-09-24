#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MORABEZACharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UMORABEZAInteractionComponent;
class AMORABEZAContactActor;
class UMORABEZAProceduralCharacterComponent;
class UAnimationAsset;

class UInputComponent;
class UInputAction;
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;

struct FInputActionValue;

UCLASS()
class MORABEZA_API AMORABEZACharacter : public ACharacter
{
    GENERATED_BODY()

public:

    AMORABEZACharacter();

    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void Interact();

    // Called only by the owning player. The server resolves and revalidates
    // every request; the passed actor is an untrusted candidate.
    UFUNCTION(Server, Reliable)
    void ServerTryInteract(AActor* RequestedTarget);

    // Cosmetic dialogue acknowledgement for an accepted test contact.
    // Never grants items, currency, missions, or other game-domain state.
    UFUNCTION(Client, Reliable)
    void ClientPresentValidatedContact(AMORABEZAContactActor* Contact);

protected:

    virtual void BeginPlay() override;

    // Runs on the owning client when the pawn is restarted/possessed.
    // BeginPlay alone is not a reliable point for local input mapping.
    virtual void PawnClientRestart() override;

    // Also reconcile a replicated controller change (including unpossession).
    virtual void OnRep_Controller() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void SetupPlayerInputComponent(
        UInputComponent* PlayerInputComponent
    ) override;

private:

    /*
     * ============================================================
     * ENHANCED INPUT
     * ============================================================
     */

    // Idempotent and local-only. Never wipes other systems' mapping contexts.
    void RefreshLocalInputMapping();
    void UnregisterLocalInputMapping();

    TWeakObjectPtr<UEnhancedInputLocalPlayerSubsystem> RegisteredInputSubsystem;

    void MoveForward(
        const FInputActionValue& Value
    );

    void MoveRight(
        const FInputActionValue& Value
    );

    void Turn(
        const FInputActionValue& Value
    );

    void LookUp(
        const FInputActionValue& Value
    );

    void StartJump(
        const FInputActionValue& Value
    );

    void StopJump(
        const FInputActionValue& Value
    );

    void StartSprint(
        const FInputActionValue& Value
    );

    void StopSprint(
        const FInputActionValue& Value
    );

    void HandleInteract(
        const FInputActionValue& Value
    );


    /*
     * ============================================================
     * INPUT ASSETS
     * ============================================================
     */

    UPROPERTY()
    TObjectPtr<UInputMappingContext> InputMappingContext;

    UPROPERTY()
    TObjectPtr<UInputAction> MoveForwardAction;

    UPROPERTY()
    TObjectPtr<UInputAction> MoveRightAction;

    UPROPERTY()
    TObjectPtr<UInputAction> TurnAction;

    UPROPERTY()
    TObjectPtr<UInputAction> LookUpAction;

    UPROPERTY()
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY()
    TObjectPtr<UInputAction> InteractAction;

    UPROPERTY()
    TObjectPtr<UInputAction> SprintAction;


    /*
     * ============================================================
     * CAMERA
     * ============================================================
     */

    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly,
        Category = "Camera",
        meta = (AllowPrivateAccess = "true")
    )
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly,
        Category = "Camera",
        meta = (AllowPrivateAccess = "true")
    )
    TObjectPtr<UCameraComponent> FollowCamera;


    /*
     * ============================================================
     * INTERACTION
     * ============================================================
     */

    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly,
        Category = "Interaction",
        meta = (AllowPrivateAccess = "true")
    )
    TObjectPtr<UMORABEZAInteractionComponent> InteractionComponent;


    /*
     * ============================================================
     * PROCEDURAL CHARACTER
     * ============================================================
     */

    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly,
        Category = "MORABEZA|Character",
        meta = (AllowPrivateAccess = "true")
    )
    TObjectPtr<UMORABEZAProceduralCharacterComponent> ProceduralCharacter;


    /*
     * ============================================================
     * ANIMATIONS
     * ============================================================
     */

    UPROPERTY()
    TObjectPtr<UAnimationAsset> IdleAnimation;

    UPROPERTY()
    TObjectPtr<UAnimationAsset> WalkAnimation;

    UPROPERTY()
    TObjectPtr<UAnimationAsset> RunAnimation;


    /*
     * ============================================================
     * MOVEMENT SETTINGS
     * ============================================================
 */

    UPROPERTY(EditAnywhere, Category = "MORABEZA|Movement")
    float WalkSpeed = 250.0f;

    UPROPERTY(EditAnywhere, Category = "MORABEZA|Movement")
    float SprintSpeed = 600.0f;
};
