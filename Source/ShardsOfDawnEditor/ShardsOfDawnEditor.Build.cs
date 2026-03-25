// Copyright Shards of Dawn. All Rights Reserved.

using UnrealBuildTool;

public class ShardsOfDawnEditor : ModuleRules
{
	public ShardsOfDawnEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"UnrealEd",
			"ShardsOfDawn"
		});
	}
}
