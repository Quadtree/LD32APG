// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "PrjHeatSeekerPatternComponent.h"
#include "LD32APGPawn.h"

#define TRACK_POWER 15000

UPrjHeatSeekerPatternComponent::UPrjHeatSeekerPatternComponent()
{
	CostModifier = 4.5f;
	Category = EPrjComponentCategory::PCC_FlightPattern;
}

void UPrjHeatSeekerPatternComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPrimitiveComponent* rt = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

	rt->SetLinearDamping(1.5f);

	float closestSqr = 10000000000000.f;

	ALD32APGPawn* trg = nullptr;

	for (TActorIterator<ALD32APGPawn> itr(GetOwner()->GetWorld()); itr; ++itr)
	{
		if (*itr == GetOwner()->GetInstigator()) continue;

		float distSqr = FVector::DistSquared(itr->GetActorLocation(), GetOwner()->GetActorLocation());

		if (distSqr < closestSqr)
		{
			trg = *itr;
			closestSqr = distSqr;
		}
	}

	if (trg)
	{
		FVector delta = (trg->GetActorLocation() + FVector::UpVector * FVector::Dist(trg->GetActorLocation(), GetOwner()->GetActorLocation()) / 10) - GetOwner()->GetActorLocation();
		delta.Normalize();
		delta *= TRACK_POWER;

		rt->AddImpulse(delta * DeltaTime, NAME_None, true);
	}
}
