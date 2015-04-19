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

	for (auto a : GetGoldOnTarget())
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ScoreParticleSystem, a->GetActorLocation());
		UGameplayStatics::PlaySoundAtLocation(this, ScoreSound, a->GetActorLocation());
		a->Destroy();
		++VictoryPoints;
	}
}

TArray<AActor*> ATeamStartZone::GetGoldOnTarget()
{
	check(this);

	TArray<AActor*> ret;

	TArray<FOverlapResult> res;

	FVector origin, extent;

	GetActorBounds(false, origin, extent);

	//UE_LOG(LogTemp, Display, TEXT("O=%s E=%s"), *origin.ToCompactString(), *extent.ToCompactString());

	if (GetWorld()->OverlapMulti(res, GetActorLocation(), GetActorRotation().Quaternion(), FCollisionShape::MakeBox((extent)+FVector::UpVector * 10000), FCollisionQueryParams(), FCollisionObjectQueryParams::AllDynamicObjects))
	{
		for (auto a : res)
		{
			if (a.Actor.IsValid() && a.Actor->FindComponentByClass<UVictoryPointComponent>()) ret.Add(a.Actor.Get());
		}
	}

	return ret;
}

int32 ATeamStartZone::GetVictoryPoints()
{
	return VictoryPoints;
}