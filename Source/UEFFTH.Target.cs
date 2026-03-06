using UnrealBuildTool;
using System.Collections.Generic;

public class UEFFTHTarget : TargetRules
{
    public UEFFTHTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
        ExtraModuleNames.AddRange(new string[] { "UEFFTH" });
    }
}
