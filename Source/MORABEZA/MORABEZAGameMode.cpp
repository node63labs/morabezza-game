#include "MORABEZAGameMode.h"

#include "MORABEZACharacter.h"
#include "MORABEZAHUD.h"
#include "MORABEZAContactActor.h"

#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

AMORABEZAGameMode::AMORABEZAGameMode()
{
    DefaultPawnClass = AMORABEZACharacter::StaticClass();
    HUDClass = AMORABEZAHUD::StaticClass();
}

void AMORABEZAGameMode::RestartPlayer(AController* NewPlayer)
{
    // AGameModeBase exists only on the authoritative game server.
    // The base method attempts to create/possess this player's pawn.
    Super::RestartPlayer(NewPlayer);

    APlayerController* PlayerController = Cast<APlayerController>(NewPlayer);
    if (!IsValid(PlayerController) || !IsValid(PlayerController->GetPawn()))
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("MORABEZA GAMEMODE: No player pawn after RestartPlayer.")
        );
        return;
    }

    const TWeakObjectPtr<AController> Key(NewPlayer);

    // Respawn should not leave duplicate private test contacts behind.
    if (TWeakObjectPtr<AMORABEZAContactActor>* Existing =
            TestContactsByController.Find(Key))
    {
        if (AMORABEZAContactActor* PreviousContact = Existing->Get())
        {
            PreviousContact->Destroy();
        }
        TestContactsByController.Remove(Key);
    }

    SpawnTestContact(PlayerController);
}

void AMORABEZAGameMode::Logout(AController* Exiting)
{
    const TWeakObjectPtr<AController> Key(Exiting);
    if (TWeakObjectPtr<AMORABEZAContactActor>* Existing =
            TestContactsByController.Find(Key))
    {
        if (AMORABEZAContactActor* Contact = Existing->Get())
        {
            Contact->Destroy();
        }
        TestContactsByController.Remove(Key);
    }

    Super::Logout(Exiting);
}

void AMORABEZAGameMode::SpawnTestContact(APlayerController* PlayerController)
{
    UWorld* World = GetWorld();
    APawn* PlayerPawn =
        IsValid(PlayerController) ? PlayerController->GetPawn() : nullptr;

    if (!IsValid(World) || !IsValid(PlayerPawn))
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("MORABEZA GAMEMODE: Cannot spawn per-player test contact.")
        );
        return;
    }

    // The dedicated server has no reliable local gameplay-camera viewpoint.
    // Place the private test contact relative to THIS player's pawn.
    FVector Forward = PlayerPawn->GetActorForwardVector();
    Forward.Z = 0.0f;
    if (!Forward.Normalize())
    {
        Forward = FVector::ForwardVector;
    }

    const FVector SpawnLocation =
        PlayerPawn->GetActorLocation() + Forward * 250.0f;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = PlayerController;
    SpawnParams.Instigator = PlayerPawn;
    SpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AMORABEZAContactActor* Contact =
        World->SpawnActor<AMORABEZAContactActor>(
            AMORABEZAContactActor::StaticClass(),
            SpawnLocation,
            FRotator::ZeroRotator,
            SpawnParams
        );

    if (!IsValid(Contact))
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("MORABEZA GAMEMODE: Test contact spawn failed.")
        );
        return;
    }

    Contact->ContactName = FText::FromString(TEXT("Test Contact"));
    Contact->MissionId = TEXT("TEST_INTERACTION");

    // This is a DEVELOPMENT-ONLY actor: each connection sees its own test
    // contact; it is not a shared quest object or a trusted reward source.
    Contact->bOnlyRelevantToOwner = true;
    Contact->SetReplicates(true);

    TestContactsByController.Add(
        TWeakObjectPtr<AController>(PlayerController),
        TWeakObjectPtr<AMORABEZAContactActor>(Contact)
    );

    UE_LOG(
        LogTemp,
        Log,
        TEXT("MORABEZA GAMEMODE: Spawned owner-only test contact for %s."),
        *PlayerController->GetName()
    );
}
