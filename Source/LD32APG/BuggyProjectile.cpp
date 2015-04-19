// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "BuggyProjectile.h"


// Sets default values
ABuggyProjectile::ABuggyProjectile(const FObjectInitializer& oi) : Super(oi)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = oi.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));

	Mesh->SetSimulatePhysics(true);

	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void ABuggyProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuggyProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

