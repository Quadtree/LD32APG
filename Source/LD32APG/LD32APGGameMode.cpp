// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "LD32APG.h"
#include "LD32APGGameMode.h"
#include "LD32APGPawn.h"

ALD32APGGameMode::ALD32APGGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = ALD32APGPawn::StaticClass();
}
