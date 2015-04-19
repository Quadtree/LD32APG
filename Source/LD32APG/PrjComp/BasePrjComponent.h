// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "BasePrjComponent.generated.h"

UENUM(BlueprintType)
enum class EPrjComponentCategory : uint8
{
	PCC_FireMode,
	PCC_Warhead,
	PCC_FlightPattern,
	PCC_Fuze,
	PCC_Max
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LD32APG_API UBasePrjComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBasePrjComponent();

	// Called when the game starts
	virtual void InitializeComponent() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	virtual void Detonate();

	UPROPERTY(EditAnywhere, Category = PrjComponent)
	EPrjComponentCategory Category;
	
	float CostModifier;
};
