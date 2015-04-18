// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "FBuggy.h"


// Sets default values
AFBuggy::AFBuggy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFBuggy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFBuggy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (FMath::Abs(Right) < 0.01f)
	{
		for (UActorComponent* comp : this->GetComponentsByClass(UPhysicsConstraintComponent::StaticClass()))
		{
			UPhysicsConstraintComponent* c = Cast<UPhysicsConstraintComponent>(comp);

			if (c)
			{
				c->SetAngularVelocityTarget(FVector(0, Forward * 50, 0));
			}
		}
	}
	else
	{
		for (UActorComponent* comp : this->GetComponentsByClass(UPhysicsConstraintComponent::StaticClass()))
		{
			UPhysicsConstraintComponent* c = Cast<UPhysicsConstraintComponent>(comp);

			if (c)
			{
				c->SetAngularVelocityTarget(FVector(0, Right * 50 * (c->ComponentHasTag("LeftMotor") ? -1 : 1), 0));
			}
		}
	}
}

// Called to bind functionality to input
void AFBuggy::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &AFBuggy::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFBuggy::MoveRight);
}

void AFBuggy::MoveForward(float forward)
{
	Forward = forward;
}

void AFBuggy::MoveRight(float right)
{
	Right = right;
}