// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BuggyAIController.generated.h"

/**
 * 
 */
UCLASS()
class LD32APG_API ABuggyAIController : public AAIController
{
	GENERATED_BODY()

	virtual void Tick(float deltaTime) override;
	
	AActor* CurrentGoldTarget;

	void HeadTowards(FVector pos);
};
