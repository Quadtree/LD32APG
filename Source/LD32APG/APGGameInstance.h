// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "APGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LD32APG_API UAPGGameInstance : public UGameInstance
{
	GENERATED_UCLASS_BODY()
public:
	UFUNCTION(BlueprintPure, Category = WeaponConfigurations)
	TArray<class UWeaponConfiguration*> GetAllWeaponConfigurations();
	
	UFUNCTION(BlueprintCallable, Category = WeaponConfigurations)
	void DeleteWeaponConfiguration(FString name);

	UFUNCTION(BlueprintCallable, Category = WeaponConfigurations)
	void SaveWeaponConfiguration(class UWeaponConfiguration* configuration);

	UFUNCTION(BlueprintPure, Category = WeaponConfigurations)
	class UWeaponConfiguration* GetWeaponConfiguration(FString name);

	UPROPERTY(BlueprintReadWrite, Category = WeaponConfigurations)
	class UWeaponConfiguration* CurrentWeaponConfiguration;
};
