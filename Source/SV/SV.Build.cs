// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SV : ModuleRules
{
    public SV(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule",
                "Niagara", "EnhancedInput", "Json", "JsonUtilities", "Slate", "SlateCore", "GeometryCache", "Chaos", "ChaosSolverEngine", 
                "GeometryCollectionEngine", "PhysicsCore", "FieldSystemEngine" });

        PrivateDependencyModuleNames.AddRange(new string[] { "VgCore", "VgFlMngmnt" });
    }
}
