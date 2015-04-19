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
	class UWeaponConfiguration* GetLastWeaponConfiguration();

	void SetLastWeaponConfiguration(class UWeaponConfiguration* weaponConfiguration);

private:
	UPROPERTY()
	FString ConfigurationName;

	UPROPERTY()
	TArray<int32> EnabledFireGroups;

	UPROPERTY()
	TArray<TSubclassOf<class UBasePrjComponent>> ProjectileComponents;
	
};
