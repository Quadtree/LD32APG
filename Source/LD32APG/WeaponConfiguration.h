// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "PrjComp/BasePrjComponent.h"
#include "WeaponConfiguration.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class LD32APG_API UWeaponConfiguration : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponConfiguration)
	FString ConfigurationName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponConfiguration)
	TArray<int32> EnabledFireGroups;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponConfiguration)
	TArray<TSubclassOf<class UBasePrjComponent>> ProjectileComponents;

	UFUNCTION(BlueprintPure, Category = WeaponConfiguration)
	float GetCostModifierOf(EPrjComponentCategory category);

	UFUNCTION(BlueprintPure, Category = WeaponConfiguration)
	float GetTotalEnergyCost();
};
