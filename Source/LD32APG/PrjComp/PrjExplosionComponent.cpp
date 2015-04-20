// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "PrjExplosionComponent.h"

void UPrjExplosionComponent::Detonate()
{
	Super::Detonate();

	TArray<FOverlapResult> res;

	if (GetOwner()->GetWorld()->OverlapMulti(res, GetOwner()->GetActorLocation(), FQuat::Identity, FCollisionShape::MakeSphere(Radius), FCollisionQueryParams(), FCollisionObjectQueryParams::AllDynamicObjects))
	{
		for (auto r : res)
		{
			if (r.Actor.IsValid())
			{
				if (r.Actor == GetOwner()) continue;

				UPrimitiveComponent* prim = Cast<UPrimitiveComponent>(r.Component.Get());

				if (prim)
				{
					prim->AddRadialImpulse(GetOwner()->GetActorLocation() + FVector::UpVector * (1000 * (Strength > 0 ? 1 : -1)), Radius, Strength * FMath::FRandRange(0.6f, 1.4f), ERadialImpulseFalloff::RIF_Linear, true);
				}
			}
		}
	}

	GetOwner()->Destroy();
}



