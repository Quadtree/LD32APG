// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PrjComp/BasePrjComponent.h"
#include "PrjExplosionComponent.generated.h"

/**
 * 
 */
UCLASS()
class LD32APG_API UPrjExplosionComponent : public UBasePrjComponent
{
	GENERATED_BODY()

public:

	float Radius;

	float Strength;

	virtual void Detonate() override;
	
	
};
