// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "PrjFuzeTimedComponent.h"

UPrjFuzeTimedComponent::UPrjFuzeTimedComponent()
{
	CostModifier = .2f;
	Category = EPrjComponentCategory::PCC_Fuze;
	Duration = 3;
}

void UPrjFuzeTimedComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Duration -= DeltaTime;

	if (Duration <= 0)
	{
		for (auto a : GetOwner()->GetComponentsByClass(UBasePrjComponent::StaticClass()))
		{
			UBasePrjComponent* comp = Cast<UBasePrjComponent>(a);

			if (comp)
				comp->Detonate();
		}
	}
}


