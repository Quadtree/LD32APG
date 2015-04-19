// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "BuggyGunComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LD32APG_API UBuggyGunComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Gun)
	int32 FireGroup;

	UPROPERTY(EditAnywhere, Category = Gun)
	TSubclassOf<class ABuggyProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = Gun)
	UPhysicalMaterial* BouncyPhysicsMaterial;

	UPROPERTY(EditAnywhere, Category = Gun)
	class UMaterial* DefaultMaterial;

	UPROPERTY(EditAnywhere, Category = Gun)
	class UMaterial* RepulsionExplosionMaterial;

	UPROPERTY(EditAnywhere, Category = Gun)
	class UMaterial* AttractionExplosionMaterial;

	UPROPERTY(EditAnywhere, Category = Gun)
	class UMaterial* RepulsionConstantMaterial;

	UPROPERTY(EditAnywhere, Category = Gun)
	class UMaterial* AttractionConstantMaterial;
	
	void Fire(class UWeaponConfiguration* weaponConfig);
};
