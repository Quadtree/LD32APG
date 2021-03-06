// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PrjComp/BasePrjComponent.h"
#include "PrjFuzeTimedComponent.generated.h"

/**
 * 
 */
UCLASS()
class LD32APG_API UPrjFuzeTimedComponent : public UBasePrjComponent
{
	GENERATED_BODY()
public:
	UPrjFuzeTimedComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float Duration;
	
	
};
