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

	//if (FMath::Abs(Right) < 0.01f)
	//{
	for (UActorComponent* comp : this->GetComponentsByClass(UPrimitiveComponent::StaticClass()))
	{
		UPrimitiveComponent* c = Cast<UPrimitiveComponent>(comp);

		if (c && (c->ComponentHasTag("LeftWheel") || c->ComponentHasTag("RightWheel")))
		{
			//if ((Forward > 0 && c->GetPhysicsAngularVelocity().Y < Forward * 250) || (Forward < 0 && c->GetPhysicsAngularVelocity().Y > Forward * 250))
			//	c->AddAngularImpulse(c->GetRightVector() * (Forward * 1200 * DeltaTime), NAME_None, true);

			Motor(c, Forward * 250, 1600);

			//UE_LOG(LogTemp, Display, TEXT("ROT %s"), *c->GetPhysicsAngularVelocity().ToCompactString());
		}
	}

	UActorComponent* bodyComponent = nullptr;

	for (UActorComponent* comp : this->GetComponentsByClass(USceneComponent::StaticClass()))
	{
		if (comp->ComponentHasTag("BodyComponent")) bodyComponent = comp;
	}

	/*if (OldRight != Right)
	{
		for (UActorComponent* comp : this->GetComponentsByClass(USceneComponent::StaticClass()))
		{
			USceneComponent* c = Cast<USceneComponent>(comp);

			if (c && (c->ComponentHasTag("Front")))
			{
				//if ((Forward > 0 && c->GetPhysicsAngularVelocity().Y < Forward * 250) || (Forward < 0 && c->GetPhysicsAngularVelocity().Y > Forward * 250))
				//	c->AddAngularImpulse(c->GetRightVector() * (Forward * 1200 * DeltaTime), NAME_None, true);

				//FRotator bodyRot = Cast<USceneComponent>(bodyComponent)->GetRelativeTransform().Rotator();

				//FRotator curRot = c->GetRelativeTransform().Rotator();

				//curRot.Quaternion().to

				//UE_LOG(LogTemp, Display, TEXT("ROT %s %s"), *bodyRot.ToString(), *curRot.ToString());



				//c->SetRelativeRotation(FRotator(0, Right * 40, 0));

				//UE_LOG(LogTemp, Display, TEXT("ARGH) %s %s"), *c->GetRelativeTransform().Rotator().ToCompactString(), *Cast<USceneComponent>(bodyComponent)->GetRelativeTransform().Rotator().ToCompactString());

				//UE_LOG(LogTemp, Display, TEXT("ROT %s"), *c->GetPhysicsAngularVelocity().ToCompactString());

				/*FRotator rot = c->GetRelativeTransform().Rotator();

				FRotator oldRot = rot;

				rot.Yaw += (Right - OldRight) * 400;

				c->SetRelativeRotation(rot);*/

				/*c->AddLocalRotation(FRotator(0, 0, (Right - OldRight) * 40));

				//UE_LOG(LogTemp, Display, TEXT("TURN %s -> %s"), *oldRot.ToString(), *rot.ToString());
			}
		}

		OldRight = Right;
	}*/

	//}
	/*else
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

				/*Motor(c, force * 250, 100000);

				
			}
		}
	}*/
}

void AFBuggy::Motor(UPrimitiveComponent* wheel, float DesiredSpeed, float force)
{
	float currentSpeed = wheel->GetComponentTransform().InverseTransformVector(wheel->GetPhysicsAngularVelocity()).Y;

	float desiredSpeed = DesiredSpeed;

	float delta = desiredSpeed - currentSpeed;

	if (FMath::Abs(delta) > 0)
		delta /= FMath::Abs(delta);

	//UE_LOG(LogTemp, Display, TEXT("%s currentSpeed=%s desiredSpeed=%s delta=%s"), *wheel->GetName(), *FString::SanitizeFloat(currentSpeed), *FString::SanitizeFloat(desiredSpeed), *FString::SanitizeFloat(delta));

	wheel->AddAngularImpulse(delta * force * wheel->GetRightVector(), NAME_None, true);
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