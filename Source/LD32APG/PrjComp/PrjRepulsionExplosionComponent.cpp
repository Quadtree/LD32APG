// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "PrjRepulsionExplosionComponent.h"
#include "BuggyProjectile.h"

UPrjRepulsionExplosionComponent::UPrjRepulsionExplosionComponent()
{
	CostModifier = 2;
	Category = EPrjComponentCategory::PCC_Warhead;
	Radius = 5000;
	Strength = 1500;
}

void UPrjRepulsionExplosionComponent::Detonate()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetOwner(), Cast<ABuggyProjectile>(GetOwner())->RepulsionExplosionParticleSystem, GetOwner()->GetActorLocation());

	UGameplayStatics::PlaySoundAtLocation(GetOwner(), Cast<ABuggyProjectile>(GetOwner())->ExplosionSound, GetOwner()->GetActorLocation());

	Super::Detonate();
}
