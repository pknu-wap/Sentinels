// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Sentinels : ModuleRules
{
	public Sentinels(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{ "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", 
			"AIModule", "NavigationSystem" });

        PrivateIncludePaths.Add("Sentinels");
    }
}
