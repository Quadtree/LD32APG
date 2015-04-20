// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "PrjRepulsionConstantComponent.h"

#define RADIUS 3000
#define STRENGTH 900

UPrjRepulsionConstantComponent::UPrjRepulsionConstantComponent()
{
	CostModifier = 2.5f;
	Category = EPrjComponentCategory::PCC_Warhead;
}

void UPrjRepulsionConstantComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TArray<FOverlapResult> res;

	if (GetOwner()->GetWorld()->OverlapMulti(res, GetOwner()->GetActorLocation(), FQuat::Identity, FCollisionShape::MakeSphere(RADIUS), FCollisionQueryParams(), FCollisionObjectQueryParams::AllDynamicObjects))
	{
		for (auto r : res)
		{
			if (r.Actor.IsValid())
			{
				if (r.Actor == GetOwner()) continue;

				UPrimitiveComponent* prim = Cast<UPrimitiveComponent>(r.Component.Get());

				if (prim)
				{
					prim->AddRadialImpulse(GetOwner()->GetActorLocation(), RADIUS, STRENGTH * DeltaTime, ERadialImpulseFalloff::RIF_Linear, true);
				}
			}
		}
	}
}


