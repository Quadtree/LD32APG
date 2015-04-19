// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "BuggyGunComponent.h"
#include "BuggyProjectile.h"
#include "WeaponConfiguration.h"
#include "PrjComp/PrjArcsPatternComponent.h"
#include "PrjComp/BasePrjComponent.h"

void UBuggyGunComponent::Fire(class UWeaponConfiguration* weaponConfig)
{
	FVector pos = GetComponentLocation() + GetComponentRotation().RotateVector(FVector::ForwardVector) * 350;
	FRotator rot = GetComponentRotation();

	ABuggyProjectile* prj = GetOwner()->GetWorld()->SpawnActor<ABuggyProjectile>(ProjectileClass, pos, rot);

	prj->Mesh->SetPhysicsLinearVelocity(GetComponentRotation().RotateVector(FVector::ForwardVector) * 12000);

	prj->Mesh->SetEnableGravity(weaponConfig->ProjectileComponents.Contains(UPrjArcsPatternComponent::StaticClass()));

	UE_LOG(LogTemp, Display, TEXT("Adding %s"), *FString::FromInt(weaponConfig->ProjectileComponents.Num()));

	for (auto a : weaponConfig->ProjectileComponents)
	{
		UE_LOG(LogTemp, Display, TEXT("Adding %s"), *a->GetName());
		UBasePrjComponent* comp = NewObject<UBasePrjComponent>(prj, a);
		comp->RegisterComponent();
	}
}


