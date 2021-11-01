// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Cataclysm : ModuleRules
{
	public Cataclysm(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Slate" });
    }
}
