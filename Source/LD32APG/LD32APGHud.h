// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "LD32APGHud.generated.h"


UCLASS(config = Game)
class ALD32APGHud : public AHUD
{
	GENERATED_BODY()

public:
	ALD32APGHud(const FObjectInitializer& ObjectInitializer);

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface
};
