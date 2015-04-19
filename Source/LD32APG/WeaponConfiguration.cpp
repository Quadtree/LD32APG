// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "WeaponConfiguration.h"

float UWeaponConfiguration::GetCostModifierOf(EPrjComponentCategory category)
{
	if (category != EPrjComponentCategory::PCC_FireMode)
	{
		float accum = 1;

		for (auto t : ProjectileComponents)
		{
			UE_LOG(LogTemp, Display, TEXT("CostMod %s"), *FString::SanitizeFloat(t->GetDefaultObject<UBasePrjComponent>()->CostModifier));
			if (t->GetDefaultObject<UBasePrjComponent>()->Category == category) accum += t->GetDefaultObject<UBasePrjComponent>()->CostModifier;
		}

		return accum;
	}
	else
	{
		return EnabledFireGroups.Num();
	}

	
}

float UWeaponConfiguration::GetTotalEnergyCost()
{
	return GetCostModifierOf(EPrjComponentCategory::PCC_FireMode) * GetCostModifierOf(EPrjComponentCategory::PCC_FlightPattern) * GetCostModifierOf(EPrjComponentCategory::PCC_Fuze) * GetCostModifierOf(EPrjComponentCategory::PCC_Warhead);
}


