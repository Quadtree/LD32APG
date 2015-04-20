// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "PrjAttractionExplosionComponent.h"
#include "BuggyProjectile.h"

UPrjAttractionExplosionComponent::UPrjAttractionExplosionComponent()
{
	CostModifier = 2;
	Category = EPrjComponentCategory::PCC_Warhead;
	Radius = 5000;
	Strength = -1000;
}

void UPrjAttractionExplosionComponent::Detonate()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetOwner(), Cast<ABuggyProjectile>(GetOwner())->AttractionExplosionParticleSystem, GetOwner()->GetActorLocation());

	UGameplayStatics::PlaySoundAtLocation(GetOwner(), Cast<ABuggyProjectile>(GetOwner())->ExplosionSound, GetOwner()->GetActorLocation());

	Super::Detonate();
}
