#include "MORABEZACharacter.h"
#include "Character/MORABEZAProceduralCharacterComponent.h"

#include "Components/CapsuleComponent.h"

#include "UObject/ConstructorHelpers.h"

#include "Engine/SkeletalMesh.h"

#include "Animation/AnimationAsset.h"

#include "Camera/CameraComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "Components/InputComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "InputCoreTypes.h"

#include "MORABEZAInteractionComponent.h"
#include "MORABEZAContactActor.h"
#include "MORABEZAHUD.h"


AMORABEZACharacter::AMORABEZACharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    SetReplicates(true);


    /*
     * ============================================================
     * CHARACTER VISUALS
     * ============================================================
     *
     * The playable character is currently provided by
     * UMORABEZAProceduralCharacterComponent.
     *
     * The previous Manny skeletal-mesh references were removed
     * because those assets are not present in this UE 5.8 install.
     */

    /*
     * ============================================================
     * ALBION-STYLE FIXED CAMERA
     * ============================================================
     *
     * Fixed, distant, elevated camera.
     *
     * NO:
     * - Mouse rotation
     * - Mouse pitch
     * - Controller camera rotation
     *
     * YES:
     * - Camera follows character position
     * - Camera remains at fixed angle
     * - Camera remains distant
     */

    CameraBoom =
        CreateDefaultSubobject<USpringArmComponent>(
            TEXT("CameraBoom")
        );

    CameraBoom->SetupAttachment(
        RootComponent
    );


    /*
     * Camera distance.
     *
     * 1100 = distant strategic/isometric view.
     */
    CameraBoom->TargetArmLength = 1100.0f;


    /*
     * Slight vertical offset.
     */
    CameraBoom->SocketOffset =
        FVector(
            0.0f,
            0.0f,
            80.0f
        );


    /*
     * Fixed high-angle camera.
     */
    CameraBoom->SetRelativeRotation(
        FRotator(
            -55.0f,
            0.0f,
            0.0f
        )
    );


    /*
     * CRITICAL:
     *
     * Never inherit controller rotation.
     */
    CameraBoom->bUsePawnControlRotation = false;

    CameraBoom->bInheritPitch = false;
    CameraBoom->bInheritYaw = false;
    CameraBoom->bInheritRoll = false;


    /*
     * Camera collision protection.
     */
    CameraBoom->bDoCollisionTest = true;
    CameraBoom->ProbeSize = 12.0f;


    /*
     * ============================================================
     * FOLLOW CAMERA
     * ============================================================
     */

    FollowCamera =
        CreateDefaultSubobject<UCameraComponent>(
            TEXT("FollowCamera")
        );

    FollowCamera->SetupAttachment(
        CameraBoom,
        USpringArmComponent::SocketName
    );

    FollowCamera->bUsePawnControlRotation = false;

    FollowCamera->SetActive(true);


    /*
     * ============================================================
     * INTERACTION COMPONENT
     * ============================================================
     */

    InteractionComponent =
        CreateDefaultSubobject<UMORABEZAInteractionComponent>(
            TEXT("InteractionComponent")
        );


    /*
     * ============================================================
     * PROCEDURAL CHARACTER
     * ============================================================
     *
     * Temporary guaranteed-visible MORABEZA character.
     * This will later be replaced by the final artist-created
     * female skeletal character.
     */

    ProceduralCharacter =
        CreateDefaultSubobject<UMORABEZAProceduralCharacterComponent>(
            TEXT("ProceduralCharacter")
        );

    ProceduralCharacter->SetupAttachment(GetCapsuleComponent());



    /*
     * ============================================================
     * CHARACTER ROTATION / MOVEMENT
     * ============================================================
     */

    bUseControllerRotationYaw = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;


    /*
     * Character faces movement direction.
     */
    GetCharacterMovement()->bOrientRotationToMovement = true;


    GetCharacterMovement()->RotationRate =
        FRotator(
            0.0f,
            720.0f,
            0.0f
        );


    /*
     * Jump.
     */
    GetCharacterMovement()->JumpZVelocity = 500.0f;

    GetCharacterMovement()->AirControl = 0.35f;


    /*
     * Normal speed.
     */
    GetCharacterMovement()->MaxWalkSpeed =
        WalkSpeed;


    /*
     * ============================================================
     * ENHANCED INPUT
     * ============================================================
     */

    InputMappingContext =
        NewObject<UInputMappingContext>(
            this,
            TEXT("MORABEZAInputMappingContext")
        );


    MoveForwardAction =
        NewObject<UInputAction>(
            this,
            TEXT("MoveForwardAction")
        );


    MoveRightAction =
        NewObject<UInputAction>(
            this,
            TEXT("MoveRightAction")
        );


    /*
     * Kept for header compatibility.
     *
     * These actions have NO mappings.
     */
    TurnAction =
        NewObject<UInputAction>(
            this,
            TEXT("TurnAction")
        );


    LookUpAction =
        NewObject<UInputAction>(
            this,
            TEXT("LookUpAction")
        );


    JumpAction =
        NewObject<UInputAction>(
            this,
            TEXT("JumpAction")
        );


    InteractAction =
        NewObject<UInputAction>(
            this,
            TEXT("InteractAction")
        );


    SprintAction =
        NewObject<UInputAction>(
            this,
            TEXT("SprintAction")
        );


    /*
     * ============================================================
     * INPUT VALUE TYPES
     * ============================================================
     */

    MoveForwardAction->ValueType =
        EInputActionValueType::Axis1D;

    MoveRightAction->ValueType =
        EInputActionValueType::Axis1D;

    TurnAction->ValueType =
        EInputActionValueType::Axis1D;

    LookUpAction->ValueType =
        EInputActionValueType::Axis1D;

    JumpAction->ValueType =
        EInputActionValueType::Boolean;

    InteractAction->ValueType =
        EInputActionValueType::Boolean;

    SprintAction->ValueType =
        EInputActionValueType::Boolean;


    /*
     * ============================================================
     * KEY MAPPINGS
     * ============================================================
     */

    if (InputMappingContext)
    {
        /*
         * --------------------------------------------------------
         * W = FORWARD
         * --------------------------------------------------------
         */

        InputMappingContext->MapKey(
            MoveForwardAction,
            EKeys::W
        );


        /*
         * --------------------------------------------------------
         * S = BACKWARD
         * --------------------------------------------------------
         */

        FEnhancedActionKeyMapping& MoveForwardS =
            InputMappingContext->MapKey(
                MoveForwardAction,
                EKeys::S
            );

        MoveForwardS.Modifiers.Add(
            NewObject<UInputModifierNegate>(
                InputMappingContext
            )
        );


        /*
         * --------------------------------------------------------
         * D = RIGHT
         * --------------------------------------------------------
         */

        InputMappingContext->MapKey(
            MoveRightAction,
            EKeys::D
        );


        /*
         * --------------------------------------------------------
         * A = LEFT
         * --------------------------------------------------------
         */

        FEnhancedActionKeyMapping& MoveRightA =
            InputMappingContext->MapKey(
                MoveRightAction,
                EKeys::A
            );

        MoveRightA.Modifiers.Add(
            NewObject<UInputModifierNegate>(
                InputMappingContext
            )
        );


        /*
         * --------------------------------------------------------
         * NO MOUSE INPUT
         * --------------------------------------------------------
         *
         * MouseX = NOT MAPPED
         * MouseY = NOT MAPPED
         *
         * Camera remains fixed.
         */


        /*
         * --------------------------------------------------------
         * SPACE = JUMP
         * --------------------------------------------------------
         */

        InputMappingContext->MapKey(
            JumpAction,
            EKeys::SpaceBar
        );


        /*
         * --------------------------------------------------------
         * E = INTERACTION / DIALOGUE
         * --------------------------------------------------------
         */

        FEnhancedActionKeyMapping& InteractE =
            InputMappingContext->MapKey(
                InteractAction,
                EKeys::E
            );

        /*
         * Explicitly make sure the mapping is active.
         */
        InteractE.bShouldBeIgnored = false;


        /*
         * --------------------------------------------------------
         * LEFT SHIFT = SPRINT
         * --------------------------------------------------------
         */

        InputMappingContext->MapKey(
            SprintAction,
            EKeys::LeftShift
        );
    }


    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA INPUT: Enhanced Input objects created."
        )
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA INPUT: E mapped to InteractAction."
        )
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA CAMERA: Fixed Albion-style camera configured."
        )
    );
}


/*
 * ================================================================
 * BEGIN PLAY
 * ================================================================
 */

void AMORABEZACharacter::BeginPlay()
{
    Super::BeginPlay();


    /*
     * ============================================================
     * CHARACTER DEBUG
     * ============================================================
     */

    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA CHARACTER SPAWNED: Location=%s Rotation=%s"
        ),
        *GetActorLocation().ToString(),
        *GetActorRotation().ToString()
    );


    /*
     * ============================================================
     * ENHANCED INPUT SUBSYSTEM
     * ============================================================
     */

    APlayerController* PlayerController =
        Cast<APlayerController>(
            GetController()
        );

    if (PlayerController)
    {
        ULocalPlayer* LocalPlayer =
            PlayerController->GetLocalPlayer();

        if (LocalPlayer)
        {
            UEnhancedInputLocalPlayerSubsystem*
                InputSubsystem =
                    LocalPlayer->GetSubsystem<
                        UEnhancedInputLocalPlayerSubsystem
                    >();

            if (InputSubsystem)
            {
                /*
                 * Remove previous contexts.
                 */
                InputSubsystem->ClearAllMappings();


                /*
                 * Add MORABEZA V1 context.
                 */
                if (InputMappingContext)
                {
                    InputSubsystem->AddMappingContext(
                        InputMappingContext,
                        0
                    );

                    UE_LOG(
                        LogTemp,
                        Warning,
                        TEXT(
                            "MORABEZA INPUT: Mapping context ACTIVE."
                        )
                    );
                }
                else
                {
                    UE_LOG(
                        LogTemp,
                        Error,
                        TEXT(
                            "MORABEZA INPUT ERROR: "
                            "InputMappingContext is NULL."
                        )
                    );
                }
            }
            else
            {
                UE_LOG(
                    LogTemp,
                    Error,
                    TEXT(
                        "MORABEZA INPUT ERROR: "
                        "Enhanced Input subsystem unavailable."
                    )
                );
            }
        }
        else
        {
            UE_LOG(
                LogTemp,
                Error,
                TEXT(
                    "MORABEZA INPUT ERROR: LocalPlayer unavailable."
                )
            );
        }
    }
    else
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "MORABEZA INPUT ERROR: PlayerController unavailable."
            )
        );
    }


    /*
     * ============================================================
     * DEBUG
     * ============================================================
     */

    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA CHARACTER CONTROLLER: %s"
        ),
        GetController()
            ? *GetController()->GetName()
            : TEXT("NONE")
    );


    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA CAMERA: ArmLength=%.1f Pitch=%.1f"
        ),
        CameraBoom
            ? CameraBoom->TargetArmLength
            : 0.0f,
        CameraBoom
            ? CameraBoom->GetRelativeRotation().Pitch
            : 0.0f
    );


    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA CAMERA: Fixed Albion-style camera ACTIVE."
        )
    );


    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA CAMERA: Mouse rotation DISABLED."
        )
    );


    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA INPUT: WASD / SPACE / E / SHIFT active."
        )
    );
}


/*
 * ================================================================
 * TICK
 * ================================================================
 */

void AMORABEZACharacter::Tick(
    float DeltaTime
)
{
    Super::Tick(DeltaTime);
}

/*
 * ================================================================
 * INPUT SETUP
 * ================================================================
 */

void AMORABEZACharacter::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent
)
{
    Super::SetupPlayerInputComponent(
        PlayerInputComponent
    );


    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA INPUT: SetupPlayerInputComponent called."
        )
    );


    UEnhancedInputComponent*
        EnhancedInputComponent =
            Cast<UEnhancedInputComponent>(
                PlayerInputComponent
            );


    if (!EnhancedInputComponent)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "MORABEZA INPUT ERROR: "
                "InputComponent is not EnhancedInputComponent."
            )
        );

        return;
    }


    /*
     * ============================================================
     * MOVEMENT
     * ============================================================
     */

    EnhancedInputComponent->BindAction(
        MoveForwardAction,
        ETriggerEvent::Triggered,
        this,
        &AMORABEZACharacter::MoveForward
    );


    EnhancedInputComponent->BindAction(
        MoveRightAction,
        ETriggerEvent::Triggered,
        this,
        &AMORABEZACharacter::MoveRight
    );


    /*
     * ============================================================
     * CAMERA
     * ============================================================
     *
     * NOTHING IS BOUND HERE.
     *
     * Mouse camera navigation is disabled.
     */


    /*
     * ============================================================
     * JUMP
     * ============================================================
     */

    EnhancedInputComponent->BindAction(
        JumpAction,
        ETriggerEvent::Started,
        this,
        &AMORABEZACharacter::StartJump
    );


    EnhancedInputComponent->BindAction(
        JumpAction,
        ETriggerEvent::Completed,
        this,
        &AMORABEZACharacter::StopJump
    );


    /*
     * ============================================================
     * INTERACTION
     * ============================================================
     */

    EnhancedInputComponent->BindAction(
        InteractAction,
        ETriggerEvent::Started,
        this,
        &AMORABEZACharacter::HandleInteract
    );


    /*
     * ============================================================
     * SPRINT
     * ============================================================
     */

    EnhancedInputComponent->BindAction(
        SprintAction,
        ETriggerEvent::Started,
        this,
        &AMORABEZACharacter::StartSprint
    );


    EnhancedInputComponent->BindAction(
        SprintAction,
        ETriggerEvent::Completed,
        this,
        &AMORABEZACharacter::StopSprint
    );


    /*
     * ============================================================
     * FINAL INPUT DEBUG
     * ============================================================
     */

    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA INPUT: Enhanced Input bindings registered."
        )
    );


    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA INPUT: E -> HandleInteract registered."
        )
    );


    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA INPUT: Mouse camera navigation DISABLED."
        )
    );
}


/*
 * ================================================================
 * MOVEMENT
 * ================================================================
 */

void AMORABEZACharacter::MoveForward(
    const FInputActionValue& Value
)
{
    const float AxisValue =
        Value.Get<float>();


    if (FMath::IsNearlyZero(AxisValue))
    {
        return;
    }

    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA MOVEMENT: MoveForward Axis=%f Controller=%s Location=%s"
        ),
        AxisValue,
        Controller ? TEXT("VALID") : TEXT("NULL"),
        *GetActorLocation().ToString()
    );

    if (!Controller)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "MORABEZA MOVEMENT ERROR: Controller is NULL."
            )
        );

        return;
    }


    /*
     * Fixed world-space movement.
     *
     * W = +X
     * S = -X
     */

    const FVector ForwardDirection =
        FVector::ForwardVector;


    AddMovementInput(
        ForwardDirection,
        AxisValue
    );
}


void AMORABEZACharacter::MoveRight(
    const FInputActionValue& Value
)
{
    const float AxisValue =
        Value.Get<float>();


    if (FMath::IsNearlyZero(AxisValue))
    {
        return;
    }

    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA MOVEMENT: MoveRight Axis=%f Controller=%s Location=%s"
        ),
        AxisValue,
        Controller ? TEXT("VALID") : TEXT("NULL"),
        *GetActorLocation().ToString()
    );

    if (!Controller)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "MORABEZA MOVEMENT ERROR: Controller is NULL."
            )
        );

        return;
    }


    /*
     * Fixed world-space movement.
     *
     * D = +Y
     * A = -Y
     */

    const FVector RightDirection =
        FVector::RightVector;


    AddMovementInput(
        RightDirection,
        AxisValue
    );
}


/*
 * ================================================================
 * CAMERA FUNCTIONS
 * ================================================================
 *
 * Retained because they exist in the header.
 *
 * They are intentionally empty.
 */

void AMORABEZACharacter::Turn(
    const FInputActionValue& Value
)
{
    /*
     * Camera rotation disabled.
     */
}


void AMORABEZACharacter::LookUp(
    const FInputActionValue& Value
)
{
    /*
     * Camera rotation disabled.
     */
}


/*
 * ================================================================
 * JUMP
 * ================================================================
 */

void AMORABEZACharacter::StartJump(
    const FInputActionValue& Value
)
{
    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA INPUT: SPACE PRESSED - JUMP"
        )
    );


    Jump();
}


void AMORABEZACharacter::StopJump(
    const FInputActionValue& Value
)
{
    StopJumping();


    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA INPUT: SPACE RELEASED"
        )
    );
}


/*
 * ================================================================
 * SPRINT
 * ================================================================
 */

void AMORABEZACharacter::StartSprint(
    const FInputActionValue& Value
)
{
    GetCharacterMovement()->MaxWalkSpeed =
        SprintSpeed;


    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA INPUT: SPRINT STARTED"
        )
    );
}


void AMORABEZACharacter::StopSprint(
    const FInputActionValue& Value
)
{
    GetCharacterMovement()->MaxWalkSpeed =
        WalkSpeed;


    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA INPUT: SPRINT STOPPED"
        )
    );
}


/*
 * ================================================================
 * INTERACTION
 * ================================================================
 */

void AMORABEZACharacter::HandleInteract(
    const FInputActionValue& Value
)
{
    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "=================================================="
        )
    );


    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA INPUT: E PRESSED"
        )
    );


    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA INPUT: HandleInteract() fired."
        )
    );


    Interact();


    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA INPUT: HandleInteract() finished."
        )
    );
}


/*
 * ================================================================
 * INTERACT
 * ================================================================
 */

void AMORABEZACharacter::Interact()
{
    /*
     * ============================================================
     * PLAYER CONTROLLER
     * ============================================================
     */

    APlayerController* PlayerController =
        Cast<APlayerController>(
            GetController()
        );


    if (PlayerController)
    {
        /*
         * ========================================================
         * HUD
         * ========================================================
         */

        AMORABEZAHUD* HUD =
            Cast<AMORABEZAHUD>(
                PlayerController->GetHUD()
            );


        /*
         * ========================================================
         * DIALOGUE ACTIVE
         * ========================================================
         *
         * E advances dialogue instead of starting a second
         * interaction.
         */

        if (
            HUD &&
            HUD->IsDialogueActive()
        )
        {
            UE_LOG(
                LogTemp,
                Warning,
                TEXT(
                    "MORABEZA INPUT: Dialogue active - advancing."
                )
            );


            HUD->AdvanceDialogue();


            return;
        }
    }


    /*
     * ============================================================
     * INTERACTION COMPONENT
     * ============================================================
     */

    if (!InteractionComponent)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "MORABEZA INPUT ERROR: InteractionComponent NULL."
            )
        );


        return;
    }


    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA INPUT: Calling TryInteract()."
        )
    );


    InteractionComponent->TryInteract();


    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA INPUT: TryInteract() finished."
        )
    );
}
 
void AMORABEZACharacter::ServerTryInteract_Implementation(
    AActor* RequestedTarget
)
{
    if (!HasAuthority() || !IsValid(InteractionComponent))
    {
        return;
    }

    // The actor's owning connection is checked by Unreal's RPC routing.
    // The interaction component independently rechecks controller,
    // current pawn, target, range, obstruction, and test-contact owner.
    InteractionComponent->ExecuteServerInteraction(RequestedTarget);
}

void AMORABEZACharacter::ClientPresentValidatedContact_Implementation(
    AMORABEZAContactActor* Contact
)
{
    if (!IsLocallyControlled() ||
        !IsValid(Contact) ||
        Contact->GetWorld() != GetWorld())
    {
        return;
    }

    Contact->PresentDialogueToLocalPlayer(this);
}
