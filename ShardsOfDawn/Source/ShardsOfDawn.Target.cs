// Copyright Shards of Dawn. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ShardsOfDawnTarget : TargetRules
{
	public ShardsOfDawnTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.AddRange(new string[] { "ShardsOfDawn" });
	}
}
