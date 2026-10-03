#include "MORABEZAContactActor.h"

#include "Components/StaticMeshComponent.h"

#include "Dialogue/MORABEZADialogueComponent.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"

#include "MORABEZAHUD.h"

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
    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA CONTACT: %s interacted / Mission: %s"
        ),
        *ContactName.ToString(),
        *MissionId.ToString()
    );

    if (!DialogueComponent)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "MORABEZA CONTACT: DialogueComponent is NULL."
            )
        );

        return;
    }

    // Resolve the interacting pawn's controller, not an unrelated player.
    APlayerController* PlayerController = nullptr;
    if (APawn* InteractingPawn = Cast<APawn>(Interactor))
    {
        PlayerController =
            Cast<APlayerController>(InteractingPawn->GetController());
    }
    else if (IsValid(Interactor))
    {
        PlayerController =
            Cast<APlayerController>(Interactor->GetInstigatorController());
    }

    if (!IsValid(PlayerController))
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("MORABEZA CONTACT: No interacting player; dialogue refused.")
        );
        return;
    }

    // This development test contact is owned by one connection. A
    // world-placed, unowned contact retains its existing local dialogue path.
    if (GetOwner() != nullptr && GetOwner() != PlayerController)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("MORABEZA CONTACT: Wrong owner; dialogue refused.")
        );
        return;
    }

    if (MissionId == FName(TEXT("TEST_INTERACTION")) && GetOwner() == nullptr)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("MORABEZA CONTACT: Missing test owner; dialogue refused.")
        );
        return;
    }

    // Dialogue widgets exist only on the owning client, not a dedicated
    // server. This local UI path grants no authoritative gameplay result.
    if (!PlayerController->IsLocalController())
    {
        return;
    }

    AMORABEZAHUD* HUD =
        Cast<AMORABEZAHUD>(
            PlayerController->GetHUD()
        );

    if (!HUD)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "MORABEZA CONTACT: MORABEZAHUD unavailable."
            )
        );

        return;
    }

    /*
     * ============================================================
     * CONNECT DIALOGUE TO HUD
     * ============================================================
     *
     * The HUD owns the single authoritative dialogue UI.
     */

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

    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA CONTACT: Dialogue connected to HUD."
        )
    );

    /*
     * ============================================================
     * SET ACTIVE DIALOGUE COMPONENT
     * ============================================================
     *
     * The HUD needs to know which dialogue component should
     * receive subsequent E-key advance commands.
     */

    HUD->SetActiveDialogueComponent(
        DialogueComponent
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "MORABEZA CONTACT: Active dialogue component assigned to HUD."
        )
    );

    /*
     * ============================================================
     * START DIALOGUE
     * ============================================================
     */

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
