// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "WeaponConfiguration.h"
#include "PrjComp/PrjArcsPatternComponent.h"
#include "PrjComp/PrjAttractionConstantComponent.h"
#include "PrjComp/PrjAttractionExplosionComponent.h"
#include "PrjComp/PrjRepulsionConstantComponent.h"
#include "PrjComp/PrjRepulsionExplosionComponent.h"
#include "PrjComp/PrjBouncePatternComponent.h"
#include "PrjComp/PrjFuzeContactComponent.h"
#include "PrjComp/PrjFuzeTimedComponent.h"
#include "PrjComp/PrjHeatSeekerPatternComponent.h"
#include "PrjComp/PrjWeavingPatternComponent.h"

float UWeaponConfiguration::GetCostModifierOf(EPrjComponentCategory category)
{
	if (category != EPrjComponentCategory::PCC_FireMode)
	{
		float accum = 1;

		for (auto t : ProjectileComponents)
		{
			//UE_LOG(LogTemp, Display, TEXT("CostMod %s"), *FString::SanitizeFloat(t->GetDefaultObject<UBasePrjComponent>()->CostModifier));
			if (t && t->GetDefaultObject<UBasePrjComponent>() && t->GetDefaultObject<UBasePrjComponent>()->Category == category) accum += t->GetDefaultObject<UBasePrjComponent>()->CostModifier;
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

void UWeaponConfiguration::SetToRandom()
{
	EnabledFireGroups.Empty();

	do
	{
		for (int32 i = 0; i < 3; i++)
		{
			if (FMath::Rand() % 4 == 0) EnabledFireGroups.Add(i);
		}
	} while (EnabledFireGroups.Num() == 0);

	bool hasWarhead = false;

	switch (FMath::RandRange(0, 3))
	{
	case 0:
		ProjectileComponents.Add(TSubclassOf<UBasePrjComponent>(UPrjRepulsionExplosionComponent::StaticClass()));
		hasWarhead = true;
		break;
	case 1:
		ProjectileComponents.Add(TSubclassOf<UBasePrjComponent>(UPrjAttractionExplosionComponent::StaticClass()));
		hasWarhead = true;
		break;
	case 2:
		ProjectileComponents.Add(TSubclassOf<UBasePrjComponent>(UPrjRepulsionConstantComponent::StaticClass()));
		break;
	case 3:
		ProjectileComponents.Add(TSubclassOf<UBasePrjComponent>(UPrjAttractionConstantComponent::StaticClass()));
		break;
	}

	switch (FMath::RandRange(0, 2))
	{
	case 0:
		ProjectileComponents.Add(TSubclassOf<UBasePrjComponent>(UPrjBouncePatternComponent::StaticClass()));
		break;
	case 1:
		ProjectileComponents.Add(TSubclassOf<UBasePrjComponent>(UPrjHeatSeekerPatternComponent::StaticClass()));
		break;
	}

	if (hasWarhead)
	{
		switch (FMath::RandRange(0, 1))
		{
		case 0:
			ProjectileComponents.Add(TSubclassOf<UBasePrjComponent>(UPrjFuzeContactComponent::StaticClass()));
			break;
		case 1:
			ProjectileComponents.Add(TSubclassOf<UBasePrjComponent>(UPrjFuzeTimedComponent::StaticClass()));
			break;
		}
	}
}
