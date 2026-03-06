using UnrealBuildTool;
using System.Collections.Generic;

public class UEFFTHEditorTarget : TargetRules
{
    public UEFFTHEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
        ExtraModuleNames.AddRange(new string[] { "UEFFTH" });
    }
}
