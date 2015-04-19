// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "BuggyGunComponent.h"
#include "BuggyProjectile.h"
#include "WeaponConfiguration.h"
#include "PrjComp/PrjArcsPatternComponent.h"
#include "PrjComp/PrjBouncePatternComponent.h"
#include "PrjComp/BasePrjComponent.h"
#include "PrjComp/PrjAttractionConstantComponent.h"
#include "PrjComp/PrjRepulsionConstantComponent.h"
#include "PrjComp/PrjAttractionExplosionComponent.h"
#include "PrjComp/PrjRepulsionExplosionComponent.h"

void UBuggyGunComponent::Fire(class UWeaponConfiguration* weaponConfig)
{
	FVector pos = GetComponentLocation() + GetComponentRotation().RotateVector(FVector::ForwardVector) * 350;
	FRotator rot = GetComponentRotation();

	FActorSpawnParameters params;
	params.Instigator = Cast<APawn>(GetOwner());

	ABuggyProjectile* prj = GetOwner()->GetWorld()->SpawnActor<ABuggyProjectile>(ProjectileClass, pos, rot, params);

	prj->Mesh->SetPhysicsLinearVelocity(GetComponentRotation().RotateVector(FVector::ForwardVector) * 12000);

	prj->Mesh->SetEnableGravity(weaponConfig->ProjectileComponents.Contains(UPrjArcsPatternComponent::StaticClass()));

	//UE_LOG(LogTemp, Display, TEXT("Adding %s"), *FString::FromInt(weaponConfig->ProjectileComponents.Num()));

	if (weaponConfig->ProjectileComponents.Contains(UPrjBouncePatternComponent::StaticClass()))
		prj->Mesh->SetPhysMaterialOverride(BouncyPhysicsMaterial);

	prj->Mesh->SetMaterial(0, DefaultMaterial);

	if (weaponConfig->ProjectileComponents.Contains(UPrjAttractionConstantComponent::StaticClass())) prj->Mesh->SetMaterial(0, AttractionConstantMaterial);
	if (weaponConfig->ProjectileComponents.Contains(UPrjRepulsionConstantComponent::StaticClass())) prj->Mesh->SetMaterial(0, RepulsionConstantMaterial);
	if (weaponConfig->ProjectileComponents.Contains(UPrjAttractionExplosionComponent::StaticClass())) prj->Mesh->SetMaterial(0, AttractionExplosionMaterial);
	if (weaponConfig->ProjectileComponents.Contains(UPrjRepulsionExplosionComponent::StaticClass())) prj->Mesh->SetMaterial(0, RepulsionExplosionMaterial);

	for (auto a : weaponConfig->ProjectileComponents)
	{
		//UE_LOG(LogTemp, Display, TEXT("Adding %s"), *a->GetName());
		if (a)
		{
			UBasePrjComponent* comp = NewObject<UBasePrjComponent>(prj, a);
			comp->RegisterComponent();
		}
	}
}


