// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "WeaponConfigurationSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class LD32APG_API UWeaponConfigurationSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<class UWeaponConfiguration*> WeaponConfigurations;
	
	
};
