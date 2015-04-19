// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PrjComp/BasePrjComponent.h"
#include "PrjWeavingPatternComponent.generated.h"

/**
 * 
 */
UCLASS()
class LD32APG_API UPrjWeavingPatternComponent : public UBasePrjComponent
{
	GENERATED_BODY()

	UPrjWeavingPatternComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	float WavePos;

	FVector OriginalRightVector;

	bool RightVectorSet;
};
