using UnrealBuildTool;
using System.Collections.Generic;

public class ShardsOfDawnTarget : TargetRules
{
    public ShardsOfDawnTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
        ExtraModuleNames.Add("ShardsOfDawn");
    }
}
