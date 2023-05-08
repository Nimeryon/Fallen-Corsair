// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FallenCorsair : ModuleRules
{
	public FallenCorsair(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "NavigationSystem", "GameplayTasks", "Niagara" });
		PublicDefinitions.AddRange(
		new string[]
		{
			"_WIN32_WINNT_WIN10_TH2",
			"_WIN32_WINNT_WIN10_RS1",
			"_WIN32_WINNT_WIN10_RS2",
			"_WIN32_WINNT_WIN10_RS3",
			"_WIN32_WINNT_WIN10_RS4",
			"_WIN32_WINNT_WIN10_RS5"
		}
		);
	}
}
