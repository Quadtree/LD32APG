// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PrjComp/PrjConstantComponent.h"
#include "PrjRepulsionConstantComponent.generated.h"

/**
 * 
 */
UCLASS()
class LD32APG_API UPrjRepulsionConstantComponent : public UPrjConstantComponent
{
	GENERATED_BODY()
	
	UPrjRepulsionConstantComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	
};
