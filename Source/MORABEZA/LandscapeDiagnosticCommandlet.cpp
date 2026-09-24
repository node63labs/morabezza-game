#include "LandscapeDiagnosticCommandlet.h"

#include "Engine/World.h"
#include "EngineUtils.h"
#include "LandscapeProxy.h"
#include "LandscapeStreamingProxy.h"
#include "LandscapeComponent.h"
#include "UObject/UObjectGlobals.h"

ULandscapeDiagnosticCommandlet::ULandscapeDiagnosticCommandlet()
{
    LogToConsole = true;
}

int32 ULandscapeDiagnosticCommandlet::Main(const FString& Params)
{
    UE_LOG(LogTemp, Display, TEXT("=============================================="));
    UE_LOG(LogTemp, Display, TEXT("MORABEZA LANDSCAPE DIAGNOSTIC"));
    UE_LOG(LogTemp, Display, TEXT("=============================================="));

    const FString MapPath = TEXT("/Game/dskjnfkljsdnf");

    UE_LOG(
        LogTemp,
        Display,
        TEXT("Loading world: %s"),
        *MapPath
    );

    UWorld* World = LoadObject<UWorld>(
        nullptr,
        *MapPath
    );

    if (!World)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("FAILED TO LOAD WORLD: %s"),
            *MapPath
        );

        return 1;
    }

    UE_LOG(
        LogTemp,
        Display,
        TEXT("WORLD LOADED: %s"),
        *World->GetName()
    );

    InspectWorld(World);

    return 0;
}

void ULandscapeDiagnosticCommandlet::InspectWorld(UWorld* World)
{
    if (!World)
    {
        return;
    }

    int32 ProxyCount = 0;
    int32 ComponentCount = 0;

    for (TActorIterator<ALandscapeStreamingProxy> It(World); It; ++It)
    {
        ALandscapeStreamingProxy* Proxy = *It;

        if (!Proxy)
        {
            continue;
        }

        ProxyCount++;

        InspectProxy(Proxy);

        ComponentCount += Proxy->LandscapeComponents.Num();
    }

    UE_LOG(LogTemp, Display, TEXT(""));
    UE_LOG(LogTemp, Display, TEXT("=============================================="));
    UE_LOG(LogTemp, Display, TEXT("SUMMARY"));
    UE_LOG(
        LogTemp,
        Display,
        TEXT("Landscape Proxies: %d"),
        ProxyCount
    );
    UE_LOG(
        LogTemp,
        Display,
        TEXT("Landscape Components: %d"),
        ComponentCount
    );
    UE_LOG(LogTemp, Display, TEXT("=============================================="));
}

void ULandscapeDiagnosticCommandlet::InspectProxy(
    ALandscapeStreamingProxy* Proxy)
{
    if (!Proxy)
    {
        return;
    }

    // Actor labels are editor-only; this commandlet is compiled by Game and
    // Server targets as well. The UObject name is available in all targets.
    const FString ProxyName = Proxy->GetName();
    const FString PackageName = Proxy->GetPackage()->GetName();
    const FString LandscapeGuid =
        Proxy->GetLandscapeGuid().ToString();

    UE_LOG(LogTemp, Display, TEXT(""));
    UE_LOG(
        LogTemp,
        Display,
        TEXT("PROXY: %s"),
        *ProxyName
    );

    UE_LOG(
        LogTemp,
        Display,
        TEXT("Package: %s"),
        *PackageName
    );

    UE_LOG(
        LogTemp,
        Display,
        TEXT("Landscape GUID: %s"),
        *LandscapeGuid
    );

    UE_LOG(
        LogTemp,
        Display,
        TEXT("Component Count: %d"),
        Proxy->LandscapeComponents.Num()
    );

    for (ULandscapeComponent* Component :
         Proxy->LandscapeComponents)
    {
        InspectComponent(Component);
    }
}

void ULandscapeDiagnosticCommandlet::InspectComponent(
    ULandscapeComponent* Component)
{
    if (!Component)
    {
        return;
    }

    const FString ComponentName = Component->GetName();
    const FString ComponentPath = Component->GetPathName();

    UE_LOG(
        LogTemp,
        Display,
        TEXT("  COMPONENT: %s"),
        *ComponentName
    );

    UE_LOG(
        LogTemp,
        Display,
        TEXT("    Path: %s"),
        *ComponentPath
    );

    UE_LOG(
        LogTemp,
        Display,
        TEXT("    Registered: %s"),
        Component->IsRegistered()
            ? TEXT("YES")
            : TEXT("NO")
    );
}
