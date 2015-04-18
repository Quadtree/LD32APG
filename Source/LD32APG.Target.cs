// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LD32APGTarget : TargetRules
{
	public LD32APGTarget(TargetInfo Target)
	{
		Type = TargetType.Game;
	}

	//
	// TargetRules interface.
	//

	public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
		OutExtraModuleNames.Add("LD32APG");
	}
}
