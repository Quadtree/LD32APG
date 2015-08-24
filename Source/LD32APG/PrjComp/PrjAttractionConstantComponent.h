// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PrjComp/PrjConstantComponent.h"
#include "PrjAttractionConstantComponent.generated.h"

/**
 * 
 */
UCLASS()
class LD32APG_API UPrjAttractionConstantComponent : public UPrjConstantComponent
{
	GENERATED_BODY()

	UPrjAttractionConstantComponent();

	static const int32 RADIUS;
	static const int32 STRENGTH;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	
};
