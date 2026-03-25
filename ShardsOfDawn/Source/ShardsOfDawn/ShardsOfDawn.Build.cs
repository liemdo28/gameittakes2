// Copyright Shards of Dawn. All Rights Reserved.

using UnrealBuildTool;

public class ShardsOfDawn : ModuleRules
{
	public ShardsOfDawn(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"Niagara",
			"UMG",
			"Slate",
			"SlateCore",
			"OnlineSubsystem",
			"OnlineSubsystemUtils"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"NetCore",
			"NavigationSystem",
			"AIModule"
		});

		// GAS requirement
		PublicIncludePaths.AddRange(new string[]
		{
			"ShardsOfDawn"
		});
	}
}
