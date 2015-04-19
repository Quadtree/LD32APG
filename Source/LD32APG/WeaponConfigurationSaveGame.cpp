// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "WeaponConfigurationSaveGame.h"
#include "WeaponConfiguration.h"

UWeaponConfiguration* UWeaponConfigurationSaveGame::GetLastWeaponConfiguration()
{
	UWeaponConfiguration* conf = NewObject<UWeaponConfiguration>();

	conf->ConfigurationName = ConfigurationName;
	conf->EnabledFireGroups = EnabledFireGroups;
	conf->ProjectileComponents = ProjectileComponents;

	return conf;
}

void UWeaponConfigurationSaveGame::SetLastWeaponConfiguration(class UWeaponConfiguration* weaponConfiguration)
{
	ConfigurationName = weaponConfiguration->ConfigurationName;
	EnabledFireGroups = weaponConfiguration->EnabledFireGroups;
	ProjectileComponents = weaponConfiguration->ProjectileComponents;
}


