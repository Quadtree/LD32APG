// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "APGGameInstance.h"
#include "WeaponConfigurationSaveGame.h"
#include "WeaponConfiguration.h"

UAPGGameInstance::UAPGGameInstance(const FObjectInitializer& oi) : Super(oi)
{
	CurrentWeaponConfiguration = NewObject<UWeaponConfiguration>();
}

TArray<class UWeaponConfiguration*> UAPGGameInstance::GetAllWeaponConfigurations()
{
	UWeaponConfigurationSaveGame* sg = Cast<UWeaponConfigurationSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("WeaponConfigurations"), 0));

	if (sg)
	{
		return sg->WeaponConfigurations;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Created new UWeaponConfigurationSaveGame"));
		sg = NewObject<UWeaponConfigurationSaveGame>();
		UGameplayStatics::SaveGameToSlot(sg, TEXT("WeaponConfigurations"), 0);

		return sg->WeaponConfigurations;
	}
}

void UAPGGameInstance::DeleteWeaponConfiguration(FString name)
{
	UWeaponConfigurationSaveGame* sg = Cast<UWeaponConfigurationSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("WeaponConfigurations"), 0));

	if (sg)
	{
		int32 index = -1;

		for (auto wc : sg->WeaponConfigurations)
		{
			if (wc->ConfigurationName == name)
			{
				break;
			}

			index++;
		}

		if (index != -1)
		{
			sg->WeaponConfigurations.RemoveAt(index);
		}

		UGameplayStatics::SaveGameToSlot(sg, TEXT("WeaponConfigurations"), 0);
	}

	if (CurrentWeaponConfiguration && CurrentWeaponConfiguration->ConfigurationName == name)
	{
		CurrentWeaponConfiguration = NewObject<UWeaponConfiguration>();
	}
}

void UAPGGameInstance::SaveWeaponConfiguration(class UWeaponConfiguration* configuration)
{
	// first delete it if it already exists
	DeleteWeaponConfiguration(configuration->ConfigurationName);

	UWeaponConfigurationSaveGame* sg = NewObject<UWeaponConfigurationSaveGame>();

	sg->WeaponConfigurations = GetAllWeaponConfigurations();

	UGameplayStatics::SaveGameToSlot(sg, TEXT("WeaponConfigurations"), 0);
}


