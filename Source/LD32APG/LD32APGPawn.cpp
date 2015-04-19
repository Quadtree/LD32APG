// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "LD32APG.h"
#include "LD32APGPawn.h"
#include "LD32APGWheelFront.h"
#include "LD32APGWheelRear.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Vehicles/WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "Engine.h"
#include "WeaponConfiguration.h"
#include "BuggyGunComponent.h"
#include "BuggyProjectile.h"
#include "TeamStartZone.h"
#include "VictoryPointComponent.h"

#define JET_POWER 1500

#ifdef HMD_INTGERATION
// Needed for VR Headset
#include "IHeadMountedDisplay.h"
#endif // HMD_INTGERATION

const FName ALD32APGPawn::LookUpBinding("LookUp");
const FName ALD32APGPawn::LookRightBinding("LookRight");

#define LOCTEXT_NAMESPACE "VehiclePawn"

ALD32APGPawn::ALD32APGPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Simulation
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(Vehicle4W->WheelSetups.Num() == 4);

	Vehicle4W->WheelSetups[0].WheelClass = ULD32APGWheelFront::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("Bone_005");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = ULD32APGWheelFront::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("Bone_008");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = ULD32APGWheelRear::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("Bone_006");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = ULD32APGWheelRear::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("Bone_007");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 200.f);
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArm->AttachTo(RootComponent);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	GrabBeamParticleSystem = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("GrabBeam"));
}

void ALD32APGPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &ALD32APGPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ALD32APGPawn::MoveRight);
	InputComponent->BindAxis("LookUp");
	InputComponent->BindAxis("LookRight");

	InputComponent->BindAxis("JetPower", this, &ALD32APGPawn::SetJetPower);

	InputComponent->BindAction("Fire", IE_Pressed, this, &ALD32APGPawn::StartFiring);
	InputComponent->BindAction("Fire", IE_Released, this, &ALD32APGPawn::StopFiring);

	InputComponent->BindAction("DetonateCommandFuzes", IE_Pressed, this, &ALD32APGPawn::DetonateCommandFuzes);

	InputComponent->BindAction("RevertToStartArea", IE_Pressed, this, &ALD32APGPawn::RevertToStartArea);

	InputComponent->BindAction("AttemptToGrabGold", IE_Pressed, this, &ALD32APGPawn::AttemptToGrabGold);
}

void ALD32APGPawn::MoveForward(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);
	CurrentThrottle = Val;
}

void ALD32APGPawn::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}


void ALD32APGPawn::Tick(float Delta)
{
	Super::Tick(Delta);

	CurrentEnergy = FMath::Clamp(CurrentEnergy + EnergyRegeneratedPerSecond * Delta, 0.f, MaxEnergy);

	float movementEnergy = FMath::Abs(CurrentThrottle) * 3.0f * Delta;

	if (movementEnergy > CurrentEnergy)
	{
		GetVehicleMovementComponent()->SetThrottleInput(0);
	}
	else
	{
		GetVehicleMovementComponent()->SetThrottleInput(CurrentThrottle);
		CurrentEnergy -= movementEnergy;
	}

	FireCooldown -= Delta;

	if (IsFiring && FireCooldown <= 0)
	{
		if (WeaponConfiguration)
		{
			float energyUsage = WeaponConfiguration->GetTotalEnergyCost();

			if (CurrentEnergy >= energyUsage)
			{
				CurrentEnergy -= energyUsage;

				UE_LOG(LogTemp, Display, TEXT("FIRE! %s"), *FString::SanitizeFloat(energyUsage));

				for (auto a : GetComponentsByClass(UBuggyGunComponent::StaticClass()))
				{
					UBuggyGunComponent* gc = Cast<UBuggyGunComponent>(a);

					if (gc && WeaponConfiguration->EnabledFireGroups.Contains(gc->FireGroup))
					{
						UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
						gc->Fire(WeaponConfiguration);
					}
				}

				FireCooldown = 0.35f;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s has no weapon configuration"), *this->GetName());
		}
	}

	if (JetPower)
	{
		float jetEnergy = FMath::Abs(JetPower) * 24.0f * Delta;

		if (jetEnergy <= CurrentEnergy)
		{
			GetMesh()->AddImpulse(FVector::UpVector * JET_POWER * JetPower * Delta, NAME_None, true);
			CurrentEnergy -= jetEnergy;
		}
	}

	if (GetActorLocation().Z < -1000){
		CurrentEnergy = MaxEnergy; RevertToStartArea();
	}

	if (CurrentlyTowedGold)
	{
		if (FVector::Dist(CurrentlyTowedGold->GetActorLocation(), GetActorLocation()) > 1000)
		{
			FVector delta = GetActorLocation() - CurrentlyTowedGold->GetActorLocation();
			delta.Normalize();

			delta *= 3000 * Delta * Cast<UPrimitiveComponent>(CurrentlyTowedGold->GetRootComponent())->GetMass();

			Cast<UPrimitiveComponent>(CurrentlyTowedGold->GetRootComponent())->AddImpulse(delta);

			GetMesh()->AddImpulse(-delta);
		}

		if (!CurrentlyTowedGold->IsValidLowLevel()) CurrentlyTowedGold = nullptr;

		GrabBeamParticleSystem->SetVisibility(true);

		GrabBeamParticleSystem->SetVectorParameter("Source", GetActorLocation());
		GrabBeamParticleSystem->SetVectorParameter("Target", CurrentlyTowedGold->GetActorLocation());
	}
	else
	{
		GrabBeamParticleSystem->SetVisibility(false);
	}
}

void ALD32APGPawn::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponConfiguration)
	{
		for (auto a : GetComponentsByClass(UBuggyGunComponent::StaticClass()))
		{
			UBuggyGunComponent* gc = Cast<UBuggyGunComponent>(a);

			if (gc)
			{
				gc->SetVisibility(WeaponConfiguration->EnabledFireGroups.Contains(gc->FireGroup));
			}
		}
	}
}

void ALD32APGPawn::RevertToStartArea()
{
	if (CurrentEnergy >= MaxEnergy * 0.9f)
	{
		for (TActorIterator<ATeamStartZone> itr(GetWorld()); itr; ++itr)
		{
			if (itr->Team == Team)
			{
				UGameplayStatics::SpawnEmitterAtLocation(this, TeleportParticleSystem, GetActorLocation());
				UGameplayStatics::PlaySoundAtLocation(this, TeleportSound, GetActorLocation());

				FVector origin, extents;

				itr->GetActorBounds(false, origin, extents);

				origin.Z = 1000;
				extents.Z = 0;

				SetActorLocation(FMath::RandPointInBox(FBox(origin - extents, origin + extents)));

				SetActorRotation(FRotator(0, Team == 0 ? 180 : 0, 0));

				GetMesh()->SetPhysicsLinearVelocity(FVector(0, 0, 0));
				GetMesh()->SetPhysicsAngularVelocity(FVector(0, 0, 0));
				CurrentEnergy = 0;
				CurrentlyTowedGold = nullptr;

				UGameplayStatics::SpawnEmitterAtLocation(this, TeleportParticleSystem, GetActorLocation());
				UGameplayStatics::PlaySoundAtLocation(this, TeleportSound, GetActorLocation());
			}
		}
	}
}

void ALD32APGPawn::StartFiring()
{
	IsFiring = true;
}

void ALD32APGPawn::StopFiring()
{
	IsFiring = false;
}

void ALD32APGPawn::DetonateCommandFuzes()
{
	for (TActorIterator<AActor> itr(GetWorld()); itr; ++itr)
	{
		if (itr->GetInstigator() == this)
		{
			for (auto a : itr->GetComponentsByClass(UBasePrjComponent::StaticClass()))
			{
				UBasePrjComponent* comp = Cast<UBasePrjComponent>(a);

				if (comp)
				{
					comp->Detonate();
				}
			}
		}
	}
}

void ALD32APGPawn::SetJetPower(float jetPower)
{
	JetPower = jetPower;
}

AActor* ALD32APGPawn::GetCurrentGrabTarget()
{
	if (!CurrentlyTowedGold)
	{
		TArray<FOverlapResult> res;

		AActor* ret = nullptr;

		if (GetWorld()->OverlapMulti(res, GetActorLocation(), FQuat::Identity, FCollisionShape::MakeSphere(1200), FCollisionQueryParams(), FCollisionObjectQueryParams::AllDynamicObjects))
		{
			float bestDistSqr = 1000000000000;

			for (auto r : res)
			{
				if (r.Actor.IsValid())
				{
					if (r.Actor == GetOwner()) continue;

					if (!r.Actor->FindComponentByClass<UVictoryPointComponent>()) continue;

					UPrimitiveComponent* prim = Cast<UPrimitiveComponent>(r.Component.Get());

					float distSqr = FVector::DistSquared(GetActorLocation(), r.Actor->GetActorLocation());

					if (prim && distSqr < bestDistSqr)
					{
						bestDistSqr = distSqr;
						ret = r.Actor.Get();
					}
				}
			}
		}

		return ret;
	}
	else
	{
		return nullptr;
	}
}

void ALD32APGPawn::AttemptToGrabGold()
{
	CurrentlyTowedGold = GetCurrentGrabTarget();

	if (CurrentlyTowedGold) UGameplayStatics::PlaySoundAtLocation(this, GrabSound, GetActorLocation());
}


#undef LOCTEXT_NAMESPACE
