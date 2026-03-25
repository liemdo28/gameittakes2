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
            "GameplayTasks"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Niagara",
            "PhysicsCore"
        });
    }
}
