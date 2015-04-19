// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "VictoryPointComponent.h"


// Sets default values for this component's properties
UVictoryPointComponent::UVictoryPointComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVictoryPointComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// ...
	
}


// Called every frame
void UVictoryPointComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (!InitialPositionSet)
	{
		InitialPosition = GetOwner()->GetActorLocation();
		InitialPositionSet = true;
	}

	if (GetOwner()->GetActorLocation().Z < -3000)
	{
		GetOwner()->SetActorLocation(InitialPosition);

		UPrimitiveComponent* rt = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

		if (rt)
		{
			rt->SetPhysicsLinearVelocity(FVector::ZeroVector);
			rt->SetPhysicsAngularVelocity(FVector::ZeroVector);
		}
	}
}

