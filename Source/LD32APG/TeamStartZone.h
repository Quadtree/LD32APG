// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TeamStartZone.generated.h"

UCLASS()
class LD32APG_API ATeamStartZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeamStartZone();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, Category = Team)
	int32 Team;
	
	UFUNCTION(BlueprintPure, Category = Win)
	int32 GetVictoryPoints();
};
