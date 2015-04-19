// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "TeamStartZone.h"
#include "VictoryPointComponent.h"


// Sets default values
ATeamStartZone::ATeamStartZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATeamStartZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeamStartZone::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

int32 ATeamStartZone::GetVictoryPoints()
{
	int32 vp = 0;

	TArray<FOverlapResult> res;

	FVector origin, extent;

	GetActorBounds(false, origin, extent);

	//UE_LOG(LogTemp, Display, TEXT("O=%s E=%s"), *origin.ToCompactString(), *extent.ToCompactString());

	if (GetWorld()->OverlapMulti(res, GetActorLocation(), GetActorRotation().Quaternion(), FCollisionShape::MakeBox((extent) + FVector::UpVector * 10000), FCollisionQueryParams(), FCollisionObjectQueryParams::AllDynamicObjects))
	{
		for (auto a : res)
		{
			if (a.Actor.IsValid() && a.Actor->FindComponentByClass<UVictoryPointComponent>()) vp++;
		}
	}

	return vp;

	/*for (TActorIterator<AActor> itr(GetWorld()); itr; ++itr)
	{
		if (itr->FindComponentByClass<UVictoryPointComponent>())
		{
			itr->FindComponentByClass<UVictoryPointComponent>()
		}
	}*/
}