using UnrealBuildTool;

// S0-W0 dedicated-server target. This adds build intent only;
// a successful Unreal Engine 5.8 compilation is still required.
public class MORABEZAServerTarget : TargetRules
{
    public MORABEZAServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
        ExtraModuleNames.Add("MORABEZA");
    }
}
