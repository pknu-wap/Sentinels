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
            "UMG", "Slate", "SlateCore",
            "OnlineSubsystem", "OnlineSubsystemUtils",
            "GeometryCollectionEngine",
         });

        PrivateIncludePaths.Add("Sentinels_LS");
    }
}
