#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MORABEZAGameMode.generated.h"

class AController;
class APlayerController;
class AMORABEZAContactActor;

UCLASS()
class MORABEZA_API AMORABEZAGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMORABEZAGameMode();

protected:
    // Invoked on the game server for each player restart, after the pawn is spawned.
    virtual void RestartPlayer(AController* NewPlayer) override;
    virtual void Logout(AController* Exiting) override;

private:
    // Development-only contact. Not a production NPC or MMO quest authority.
    void SpawnTestContact(APlayerController* PlayerController);

    TMap<
        TWeakObjectPtr<AController>,
        TWeakObjectPtr<AMORABEZAContactActor>
    > TestContactsByController;
};
