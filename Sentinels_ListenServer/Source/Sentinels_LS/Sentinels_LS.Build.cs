// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Sentinels_LS : ModuleRules
{
	public Sentinels_LS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
         { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
            "AIModule", "NavigationSystem",
            "GameplayTags",
            "OnlineSubsystem", "OnlineSubsystemUtils",
         });

        PrivateIncludePaths.Add("Sentinels_LS");
    }
}
