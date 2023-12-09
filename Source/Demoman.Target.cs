// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DemomanTarget : TargetRules
{
	public DemomanTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
        bUseUnityBuild = false;
        bUsePCHFiles = false;
        DefaultBuildSettings = BuildSettingsVersion.V4;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.AddRange( new string[] { "Demoman" } );
	}
}
