#include "MORABEZAInteractionComponent.h"

#include "MORABEZAInteractable.h"
#include "MORABEZACharacter.h"
#include "MORABEZAContactActor.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UMORABEZAInteractionComponent::UMORABEZAInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    InteractionDistance = 350.0f;
    InteractionTraceDistance = 2000.0f;
}

void UMORABEZAInteractionComponent::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("MORABEZA INTERACTION: Component started.")
    );

    UpdateInteractionTarget();
}

void UMORABEZAInteractionComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction
)
{
    Super::TickComponent(
        DeltaTime,
        TickType,
        ThisTickFunction
    );

    UpdateInteractionTarget();
}

void UMORABEZAInteractionComponent::SetInteractionTarget(
    AActor* NewTarget
)
{
    AActor* PreviousTarget = CurrentTarget.Get();

    if (PreviousTarget == NewTarget)
    {
        return;
    }

    CurrentTarget = NewTarget;

    if (NewTarget)
    {
        const FText Prompt =
            IMORABEZAInteractable::Execute_GetInteractionPrompt(
                NewTarget
            );

        UE_LOG(
            LogTemp,
            Warning,
            TEXT(
                "MORABEZA INTERACTION: TARGET ACQUIRED - %s"
            ),
            *NewTarget->GetName()
        );

        UE_LOG(
            LogTemp,
            Warning,
            TEXT(
                "MORABEZA INTERACTION: Prompt = %s"
            ),
            *Prompt.ToString()
        );

        OnPromptChanged.Broadcast(Prompt);

        return;
    }

    if (PreviousTarget)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT(
                "MORABEZA INTERACTION: TARGET LOST - %s"
            ),
            *PreviousTarget->GetName()
        );
    }

    OnPromptChanged.Broadcast(
        FText::GetEmpty()
    );
}

void UMORABEZAInteractionComponent::UpdateInteractionTarget()
{
    ACharacter* Character =
        Cast<ACharacter>(GetOwner());

    // Client-side target detection is only a UI hint; remote simulated pawns
    // must not trace, broadcast prompts, or initiate interactions.
    if (!Character || !Character->IsLocallyControlled())
    {
        SetInteractionTarget(nullptr);
        return;
    }

    UWorld* World = GetWorld();

    if (!World)
    {
        SetInteractionTarget(nullptr);
        return;
    }

    /*
     * ============================================================
     * MORABEZA ALBION-STYLE INTERACTION
     * ============================================================
     *
     * The gameplay camera is intentionally positioned high and
     * angled downward.
     *
     * Therefore interaction does NOT use the camera center ray.
     *
     * Interaction follows the character's horizontal forward
     * direction instead.
     *
     * This makes interaction independent from the camera pitch.
     */

    const FVector CharacterLocation =
        Character->GetActorLocation();

    FVector Forward =
        Character->GetActorForwardVector();

    /*
     * Keep interaction horizontal.
     */

    Forward.Z = 0.0f;

    if (!Forward.Normalize())
    {
        SetInteractionTarget(nullptr);
        return;
    }

    /*
     * Start the sweep slightly above the character's origin.
     */

    const FVector TraceStart =
        CharacterLocation +
        FVector(0.0f, 0.0f, 60.0f);

    const FVector TraceEnd =
        TraceStart +
        (
            Forward *
            InteractionTraceDistance
        );

    FHitResult HitResult;

    FCollisionQueryParams QueryParams(
        SCENE_QUERY_STAT(MORABEZAInteractionTrace),
        true,
        Character
    );

    QueryParams.bTraceComplex = true;

    /*
     * Sphere sweep gives the player a forgiving interaction area
     * instead of requiring pixel-perfect alignment.
     */

    const float SweepRadius = 65.0f;

    const bool bHit =
        World->SweepSingleByChannel(
            HitResult,
            TraceStart,
            TraceEnd,
            FQuat::Identity,
            ECC_Visibility,
            FCollisionShape::MakeSphere(SweepRadius),
            QueryParams
        );

    /*
     * Temporary debug line.
     *
     * Green = something was hit.
     * Red   = nothing was hit.
     */

    DrawDebugLine(
        World,
        TraceStart,
        TraceEnd,
        bHit
            ? FColor::Green
            : FColor::Red,
        false,
        0.05f,
        0,
        2.0f
    );

    if (!bHit)
    {
        SetInteractionTarget(nullptr);
        return;
    }

    AActor* HitActor =
        HitResult.GetActor();

    if (!HitActor)
    {
        SetInteractionTarget(nullptr);
        return;
    }

    /*
     * ============================================================
     * INTERACTABLE CHECK
     * ============================================================
     */

    if (
        !HitActor->GetClass()->ImplementsInterface(
            UMORABEZAInteractable::StaticClass()
        )
    )
    {
        SetInteractionTarget(nullptr);
        return;
    }

    /*
     * ============================================================
     * PLAYER DISTANCE CHECK
     * ============================================================
     */

    const float DistanceSquared =
        FVector::DistSquared(
            CharacterLocation,
            HitActor->GetActorLocation()
        );

    const float MaxDistanceSquared =
        FMath::Square(InteractionDistance);

    if (DistanceSquared > MaxDistanceSquared)
    {
        SetInteractionTarget(nullptr);
        return;
    }

    SetInteractionTarget(HitActor);
}


void UMORABEZAInteractionComponent::TryInteract()
{
    AMORABEZACharacter* Character =
        Cast<AMORABEZACharacter>(GetOwner());

    if (!IsValid(Character) || !Character->IsLocallyControlled())
    {
        return;
    }

    // The local trace selects a candidate for the UI. The server will NOT
    // trust this cached target, the local hit result, or client editables.
    UpdateInteractionTarget();

    AActor* RequestedTarget = CurrentTarget.Get();
    if (!IsValid(RequestedTarget))
    {
        return;
    }

    if (Character->HasAuthority())
    {
        // Standalone or locally controlled listen-server player.
        ExecuteServerInteraction(RequestedTarget);
    }
    else
    {
        // The RPC is on the possessed, replicated character; only its
        // owning connection may invoke it. The server validates the target.
        Character->ServerTryInteract(RequestedTarget);
    }
}

void UMORABEZAInteractionComponent::ExecuteServerInteraction(
    AActor* RequestedTarget
)
{
    AMORABEZACharacter* Character =
        Cast<AMORABEZACharacter>(GetOwner());

    if (!IsValid(Character) || !Character->HasAuthority())
    {
        return;
    }

    APlayerController* PlayerController =
        Cast<APlayerController>(Character->GetController());

    if (!IsValid(PlayerController) ||
        PlayerController->GetPawn() != Character)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!IsValid(World))
    {
        return;
    }

    // Rate limiting occurs on the server, independently of client input.
    // No durable inventory/reward action is allowed through this prototype.
    const double Now = World->GetTimeSeconds();
    if (Now - LastServerInteractionSeconds < 0.35)
    {
        return;
    }
    LastServerInteractionSeconds = Now;

    if (!IsValid(RequestedTarget) ||
        RequestedTarget == Character ||
        RequestedTarget->GetWorld() != World ||
        !RequestedTarget->GetClass()->ImplementsInterface(
            UMORABEZAInteractable::StaticClass()
        ))
    {
        return;
    }

    // The first networked slice accepts ONLY each player's owner-scoped,
    // replicated test contact. This does not grant arbitrary Blueprint
    // interactables authority over rewards, missions, or inventory.
    if (World->GetNetMode() != NM_Standalone)
    {
        const AMORABEZAContactActor* Contact =
            Cast<AMORABEZAContactActor>(RequestedTarget);

        if (!IsValid(Contact) ||
            Contact->MissionId != FName(TEXT("TEST_INTERACTION")) ||
            Contact->GetOwner() != PlayerController ||
            !Contact->GetIsReplicated())
        {
            return;
        }
    }

    // InteractionDistance can be modified locally for UI purposes.
    // The authoritative server imposes an independent hard cap.
    if (!FMath::IsFinite(InteractionDistance))
    {
        return;
    }

    const float MaxDistance =
        FMath::Min(InteractionDistance, 350.0f);

    if (MaxDistance <= 0.0f ||
        FVector::DistSquared(
            Character->GetActorLocation(),
            RequestedTarget->GetActorLocation()
        ) > FMath::Square(MaxDistance))
    {
        return;
    }

    FVector Forward = Character->GetActorForwardVector();
    Forward.Z = 0.0f;
    if (!Forward.Normalize())
    {
        return;
    }

    const FVector Start =
        Character->GetActorLocation() + FVector(0.0f, 0.0f, 60.0f);
    const FVector End = Start + Forward * MaxDistance;

    FCollisionQueryParams Params(
        SCENE_QUERY_STAT(MORABEZAServerInteraction),
        true,
        Character
    );
    Params.bTraceComplex = true;

    FHitResult SweepHit;
    const bool bFound = World->SweepSingleByChannel(
        SweepHit,
        Start,
        End,
        FQuat::Identity,
        ECC_Visibility,
        FCollisionShape::MakeSphere(65.0f),
        Params
    );

    // A candidate must be the *first* blocking actor found by the
    // authoritative server; a client-supplied pointer alone is never enough.
    if (!bFound || SweepHit.GetActor() != RequestedTarget)
    {
        return;
    }

    // Prevent interactions through a thin obstacle that a forgiving sphere
    // sweep could otherwise reach around.
    FHitResult LineHit;
    const bool bLineBlocked = World->LineTraceSingleByChannel(
        LineHit,
        Start,
        SweepHit.ImpactPoint,
        ECC_Visibility,
        Params
    );
    if (bLineBlocked && LineHit.GetActor() != RequestedTarget)
    {
        return;
    }

    // Only the server invokes an interface that could eventually carry
    // gameplay effects. In this slice, its only networked implementation
    // is a dev contact that sends an owner-only cosmetic dialogue response.
    IMORABEZAInteractable::Execute_Interact(
        RequestedTarget,
        Character
    );
}
