// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "PrjFuzeContactComponent.h"

UPrjFuzeContactComponent::UPrjFuzeContactComponent()
{
	CostModifier = .8f;
	Category = EPrjComponentCategory::PCC_Fuze;
}

void UPrjFuzeContactComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetOwner()->OnActorHit.IsAlreadyBound(this, &UPrjFuzeContactComponent::OnParentHit))
		GetOwner()->OnActorHit.AddDynamic(this, &UPrjFuzeContactComponent::OnParentHit);
}

void UPrjFuzeContactComponent::OnUnregister()
{
	Super::OnUnregister();

	if (GetOwner()->OnActorHit.IsAlreadyBound(this, &UPrjFuzeContactComponent::OnParentHit))
		GetOwner()->OnActorHit.RemoveDynamic(this, &UPrjFuzeContactComponent::OnParentHit);
}

void UPrjFuzeContactComponent::OnParentHit(class AActor* selfActor, class AActor* otherActor, FVector normalImpulse, const FHitResult& hit)
{
	for (auto a : GetOwner()->GetComponentsByClass(UBasePrjComponent::StaticClass()))
	{
		UBasePrjComponent* comp = Cast<UBasePrjComponent>(a);

		if (comp)
			comp->Detonate();
	}
}


