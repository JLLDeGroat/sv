// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SV : ModuleRules
{
	public SV(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "Json", "JsonUtilities" });

        PrivateDependencyModuleNames.AddRange(new string[] { "VgCore", "VgFlMngmnt" });
    }
}
