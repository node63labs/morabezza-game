#include "MORABEZAContactActor.h"

#include "Components/StaticMeshComponent.h"

#include "Dialogue/MORABEZADialogueComponent.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"

#include "MORABEZAHUD.h"
#include "MORABEZACharacter.h"

#include "UObject/ConstructorHelpers.h"

AMORABEZAContactActor::AMORABEZAContactActor()
{
    PrimaryActorTick.bCanEverTick = false;

    /*
     * ============================================================
     * CONTACT MESH
     * ============================================================
     */

    Mesh =
        CreateDefaultSubobject<UStaticMeshComponent>(
            TEXT("Mesh")
        );

    RootComponent = Mesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh>
        MeshAsset(
            TEXT("/Engine/BasicShapes/Cube.Cube")
        );

    if (MeshAsset.Succeeded())
    {
        Mesh->SetStaticMesh(
            MeshAsset.Object
        );
    }

    Mesh->SetCollisionEnabled(
        ECollisionEnabled::QueryAndPhysics
    );

    Mesh->SetCollisionObjectType(
        ECC_WorldDynamic
    );

    Mesh->SetCollisionResponseToAllChannels(
        ECR_Block
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA CONTACT: Collision configured as WorldDynamic."
        )
    );

    /*
     * ============================================================
     * DIALOGUE COMPONENT
     * ============================================================
     */

    DialogueComponent =
        CreateDefaultSubobject<UMORABEZADialogueComponent>(
            TEXT("DialogueComponent")
        );
}

void AMORABEZAContactActor::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps
) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AMORABEZAContactActor, MissionId);
    DOREPLIFETIME(AMORABEZAContactActor, ContactName);
}

void AMORABEZAContactActor::Interact_Implementation(
    AActor* Interactor
)
{
    // A local client may never execute a game-domain interaction itself.
    // This prototype's accepted effect is an owner-only UI acknowledgement.
    if (!HasAuthority() || !IsValid(DialogueComponent))
    {
        return;
    }

    APawn* InteractingPawn = Cast<APawn>(Interactor);
    APlayerController* PlayerController =
        IsValid(InteractingPawn)
            ? Cast<APlayerController>(InteractingPawn->GetController())
            : nullptr;

    if (!IsValid(PlayerController) ||
        (GetOwner() != nullptr && GetOwner() != PlayerController))
    {
        return;
    }

    if (MissionId == FName(TEXT("TEST_INTERACTION")) &&
        GetOwner() != PlayerController)
    {
        return;
    }

    if (PlayerController->IsLocalController())
    {
        // Standalone/listen-server owner: local UI, no network round trip.
        PresentDialogueToLocalPlayer(Interactor);
        return;
    }

    // On a dedicated server there is no client HUD. Send a cosmetic
    // notification on the validated player's possessed character.
    if (AMORABEZACharacter* PlayerCharacter =
            Cast<AMORABEZACharacter>(InteractingPawn))
    {
        if (GetIsReplicated())
        {
            PlayerCharacter->ClientPresentValidatedContact(this);
        }
    }
}

void AMORABEZAContactActor::PresentDialogueToLocalPlayer(
    AActor* Interactor
)
{
    if (!IsValid(DialogueComponent))
    {
        return;
    }

    APawn* InteractingPawn = Cast<APawn>(Interactor);
    APlayerController* PlayerController =
        IsValid(InteractingPawn)
            ? Cast<APlayerController>(InteractingPawn->GetController())
            : nullptr;

    if (!IsValid(PlayerController) ||
        !PlayerController->IsLocalController() ||
        (GetOwner() != nullptr && GetOwner() != PlayerController))
    {
        return;
    }

    if (MissionId == FName(TEXT("TEST_INTERACTION")) &&
        GetOwner() != PlayerController)
    {
        return;
    }

    AMORABEZAHUD* HUD =
        Cast<AMORABEZAHUD>(PlayerController->GetHUD());

    if (!IsValid(HUD))
    {
        return;
    }

    // This actor carries only fixed prototype dialogue. Do not call an
    // interaction interface or modify authoritative gameplay state here.
    DialogueComponent->OnDialogueLineChanged.RemoveAll(HUD);
    DialogueComponent->OnDialogueFinished.RemoveAll(HUD);

    DialogueComponent->OnDialogueLineChanged.AddDynamic(
        HUD,
        &AMORABEZAHUD::HandleDialogueLineChanged
    );
    DialogueComponent->OnDialogueFinished.AddDynamic(
        HUD,
        &AMORABEZAHUD::HandleDialogueFinished
    );

    HUD->SetActiveDialogueComponent(DialogueComponent);
    DialogueComponent->StartDialogue();
}

FText AMORABEZAContactActor::GetInteractionPrompt_Implementation()
    const
{
    return FText::Format(
        FText::FromString(
            TEXT("[E] Falar com {0}")
        ),
        ContactName
    );
}
