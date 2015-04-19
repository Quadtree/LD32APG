// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "PrjWeavingPatternComponent.h"

#define HERTZ			25
#define WEAVE_POWER		1500

UPrjWeavingPatternComponent::UPrjWeavingPatternComponent()
{
	CostModifier = -0.2f;
	Category = EPrjComponentCategory::PCC_FlightPattern;
}

void UPrjWeavingPatternComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!RightVectorSet){
		OriginalRightVector = GetOwner()->GetActorRightVector(); WavePos = 1.5f;
		RightVectorSet = true;
	}

	WavePos += DeltaTime * HERTZ;

	UPrimitiveComponent* rt = CastChecked<UPrimitiveComponent>(GetOwner()->GetRootComponent());

	rt->AddImpulse(OriginalRightVector * FMath::Sin(WavePos) * WEAVE_POWER, NAME_None, true);
}
