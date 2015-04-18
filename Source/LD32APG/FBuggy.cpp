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

	/*if (FMath::Abs(Right) < 0.01f)
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
	}*/

	if (FMath::Abs(Right) < 0.01f)
	{
		for (UActorComponent* comp : this->GetComponentsByClass(UPrimitiveComponent::StaticClass()))
		{
			UPrimitiveComponent* c = Cast<UPrimitiveComponent>(comp);

			if (c && (c->ComponentHasTag("LeftWheel") || c->ComponentHasTag("RightWheel")))
			{
				//if ((Forward > 0 && c->GetPhysicsAngularVelocity().Y < Forward * 250) || (Forward < 0 && c->GetPhysicsAngularVelocity().Y > Forward * 250))
				//	c->AddAngularImpulse(c->GetRightVector() * (Forward * 1200 * DeltaTime), NAME_None, true);

				Motor(c, Forward * 250);

				//UE_LOG(LogTemp, Display, TEXT("ROT %s"), *c->GetPhysicsAngularVelocity().ToCompactString());
			}
		}
	}
	else
	{
		for (UActorComponent* comp : this->GetComponentsByClass(UPrimitiveComponent::StaticClass()))
		{
			UPrimitiveComponent* c = Cast<UPrimitiveComponent>(comp);

			if (c && (c->ComponentHasTag("LeftWheel") || c->ComponentHasTag("RightWheel")))
			{
				float force = (c->ComponentHasTag("LeftWheel") ? 1 : -1) * Right;

				/*if ((force > 0 && c->GetPhysicsAngularVelocity().Y < force * 250) || (force < 0 && c->GetPhysicsAngularVelocity().Y > force * 250))
				{
					//c->AddAngularImpulse(FVector(0, force * 1200 * DeltaTime, 0), NAME_None, true);
					c->AddAngularImpulse(c->GetRightVector() * (force * 1200 * DeltaTime), NAME_None, true);
					UE_LOG(LogTemp, Display, TEXT("ROT %s"), *c->GetPhysicsAngularVelocity().ToCompactString());
				}*/

				Motor(c, force * 250);

				
			}
		}
	}
}

void AFBuggy::Motor(UPrimitiveComponent* wheel, float DesiredSpeed)
{
	FVector currentSpeed = wheel->GetPhysicsAngularVelocity();

	FVector desiredSpeed = wheel->GetRightVector() * DesiredSpeed;

	FVector delta = desiredSpeed - currentSpeed;
	delta.Normalize();

	UE_LOG(LogTemp, Display, TEXT("currentSpeed=%s desiredSpeed=%s delta=%s"), *currentSpeed.ToString(), *desiredSpeed.ToString(), *delta.ToString());

	wheel->AddAngularImpulse(delta * 1600, NAME_None, true);
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