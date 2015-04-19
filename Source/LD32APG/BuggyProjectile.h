// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BuggyProjectile.generated.h"

UCLASS()
class LD32APG_API ABuggyProjectile : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, Category = Projectile)
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, Category = Pawn)
	UParticleSystem* RepulsionExplosionParticleSystem;

	UPROPERTY(EditAnywhere, Category = Pawn)
	UParticleSystem* AttractionExplosionParticleSystem;
};
