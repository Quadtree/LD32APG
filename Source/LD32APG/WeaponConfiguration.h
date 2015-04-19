// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "WeaponConfiguration.generated.h"

/**
 * 
 */
UCLASS()
class LD32APG_API UWeaponConfiguration : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WeaponConfiguration)
	TArray<int32> EnabledFireGroups;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WeaponConfiguration)
	TSubclassOf<class UBasePrjComponent> ProjectileComponents;
};
