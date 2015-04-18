// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LD32APGEditorTarget : TargetRules
{
	public LD32APGEditorTarget(TargetInfo Target)
	{
		Type = TargetType.Editor;
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
