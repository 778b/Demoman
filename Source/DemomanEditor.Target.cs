// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DemomanEditorTarget : TargetRules
{
	public DemomanEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        bUseUnityBuild = false;
        bUsePCHFiles = false;
        DefaultBuildSettings = BuildSettingsVersion.V4;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.AddRange( new string[] { "Demoman" } );
	}
}
