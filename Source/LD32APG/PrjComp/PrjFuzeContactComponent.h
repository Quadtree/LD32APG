// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PrjComp/BasePrjComponent.h"
#include "PrjFuzeContactComponent.generated.h"

/**
 * 
 */
UCLASS()
class LD32APG_API UPrjFuzeContactComponent : public UBasePrjComponent
{
	GENERATED_BODY()

	virtual void OnRegister() override;
	
	virtual void OnUnregister() override;

	UFUNCTION()
	virtual void OnParentHit(class AActor* selfActor, class AActor* otherActor, FVector normalImpulse, const FHitResult& hit);

	UPrjFuzeContactComponent();
};
