// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DemomanTarget : TargetRules
{
	public DemomanTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
        ExtraModuleNames.AddRange( new string[] { "Demoman" } );
	}
}
