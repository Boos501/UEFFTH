// Copyright (c) 2024 Boos501. All Rights Reserved.

using UnrealBuildTool;

public class InstancePainterEditor : ModuleRules
{
    public InstancePainterEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "EditorStyle",
                "UnrealEd",
                "InputCore",
                "LevelEditor",
                "PropertyEditor",
                "EditorFramework",
                "ToolMenus",
                "InstancePainterRuntime",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
            }
        );
    }
}
