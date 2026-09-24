#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "LandscapeDiagnosticCommandlet.generated.h"

class UWorld;
class ALandscapeStreamingProxy;
class ULandscapeComponent;

UCLASS()
class MORABEZA_API ULandscapeDiagnosticCommandlet : public UCommandlet
{
    GENERATED_BODY()

public:
    ULandscapeDiagnosticCommandlet();

    virtual int32 Main(const FString& Params) override;

private:
    void InspectWorld(UWorld* World);
    void InspectProxy(ALandscapeStreamingProxy* Proxy);
    void InspectComponent(ULandscapeComponent* Component);
};
