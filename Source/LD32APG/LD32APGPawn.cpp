// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "LD32APG.h"
#include "LD32APGPawn.h"
#include "LD32APGWheelFront.h"
#include "LD32APGWheelRear.h"
#include "LD32APGHud.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Vehicles/WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "Engine.h"

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
	// Car mesh
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/Vehicle/Sedan/Sedan_SkelMesh.Sedan_SkelMesh"));
	//GetMesh()->SetSkeletalMesh(CarMesh.Object);

	//static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Vehicle/Sedan/Sedan_AnimBP"));
	//GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);

	// Simulation
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(Vehicle4W->WheelSetups.Num() == 4);

	Vehicle4W->WheelSetups[0].WheelClass = ULD32APGWheelFront::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("FrontLeftWheel");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = ULD32APGWheelFront::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("FrontRightWheel");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = ULD32APGWheelRear::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("RearLeftWheel");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = ULD32APGWheelRear::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("RearRightWheel");
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

	// Create In-Car camera component 
	InternalCameraOrigin = FVector(8.0f, -40.0f, 130.0f);
	InternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InternalCamera"));
	InternalCamera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	InternalCamera->bUsePawnControlRotation = false;
	InternalCamera->FieldOfView = 90.f;
	InternalCamera->SetRelativeLocation(InternalCameraOrigin);
	InternalCamera->AttachTo(GetMesh());

	// Create text render component for in car speed display
	InCarSpeed = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarSpeed"));
	InCarSpeed->SetRelativeLocation(FVector(70.0f, -75.0f, 99.0f));
	InCarSpeed->SetRelativeRotation(FRotator(18.0f, 180.0f, 0.0f));
	InCarSpeed->AttachTo(GetMesh());
	InCarSpeed->SetRelativeScale3D(FVector(1.0f, 0.4f, 0.4f));

	// Create text render component for in car gear display
	InCarGear = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
	InCarGear->SetRelativeLocation(FVector(66.0f, -9.0f, 95.0f));	
	InCarGear->SetRelativeRotation(FRotator(25.0f, 180.0f,0.0f));
	InCarGear->SetRelativeScale3D(FVector(1.0f, 0.4f, 0.4f));
	InCarGear->AttachTo(GetMesh());
	
	// Colors for the incar gear display. One for normal one for reverse
	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	// Colors for the in-car gear display. One for normal one for reverse
	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	bInReverseGear = false;
}

void ALD32APGPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &ALD32APGPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ALD32APGPawn::MoveRight);
	InputComponent->BindAxis("LookUp");
	InputComponent->BindAxis("LookRight");

	InputComponent->BindAction("Handbrake", IE_Pressed, this, &ALD32APGPawn::OnHandbrakePressed);
	InputComponent->BindAction("Handbrake", IE_Released, this, &ALD32APGPawn::OnHandbrakeReleased);
	InputComponent->BindAction("SwitchCamera", IE_Pressed, this, &ALD32APGPawn::OnToggleCamera);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &ALD32APGPawn::OnResetVR); 
}

void ALD32APGPawn::MoveForward(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);
}

void ALD32APGPawn::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void ALD32APGPawn::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void ALD32APGPawn::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void ALD32APGPawn::OnToggleCamera()
{
	EnableIncarView(!bInCarCameraActive);
}

void ALD32APGPawn::EnableIncarView(const bool bState, const bool bForce)
{
	if ((bState != bInCarCameraActive) || ( bForce == true ))
	{
		bInCarCameraActive = bState;
		
		if (bState == true)
		{
			OnResetVR();
			Camera->Deactivate();
			InternalCamera->Activate();
			
			APlayerController* PlayerController = Cast<APlayerController>(GetController());
			if ( (PlayerController != nullptr) && (PlayerController->PlayerCameraManager != nullptr ) )
			{
				PlayerController->PlayerCameraManager->bFollowHmdOrientation = true;
			}
		}
		else
		{
			InternalCamera->Deactivate();
			Camera->Activate();
		}
		
		InCarSpeed->SetVisibility(bInCarCameraActive);
		InCarGear->SetVisibility(bInCarCameraActive);
	}
}


void ALD32APGPawn::Tick(float Delta)
{
	// Setup the flag to say we are in reverse gear
	bInReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;
	
	// Update the strings used in the hud (incar and onscreen)
	UpdateHUDStrings();

	// Set the string in the incar hud
	SetupInCarHUD();

#ifdef HMD_INTGERATION
	if ((GEngine->HMDDevice.IsValid() == false) || ((GEngine->HMDDevice.IsValid() == true ) && ( (GEngine->HMDDevice->IsHeadTrackingAllowed() == false) || (GEngine->IsStereoscopic3D() == false))))
	{
		if ( (InputComponent) && (bInCarCameraActive == true ))
		{
			FRotator HeadRotation = InternalCamera->RelativeRotation;
			HeadRotation.Pitch += InputComponent->GetAxisValue(LookUpBinding);
			HeadRotation.Yaw += InputComponent->GetAxisValue(LookRightBinding);
			InternalCamera->RelativeRotation = HeadRotation;
		}
	}	
#endif // HMD_INTGERATION
}

void ALD32APGPawn::BeginPlay()
{
	bool bEnableInCar = false;
#ifdef HMD_INTGERATION
	bEnableInCar = GEngine->HMDDevice.IsValid();	
#endif // HMD_INTGERATION
	EnableIncarView(bEnableInCar,true);
}

void ALD32APGPawn::OnResetVR()
{
#ifdef HMD_INTGERATION
	if (GEngine->HMDDevice.IsValid())
	{
		GEngine->HMDDevice->ResetOrientationAndPosition();
		InternalCamera->SetRelativeLocation(InternalCameraOrigin);
		GetController()->SetControlRotation(FRotator());
	}
#endif // HMD_INTGERATION
}

void ALD32APGPawn::UpdateHUDStrings()
{
	float KPH = FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) * 0.036f;
	int32 KPH_int = FMath::FloorToInt(KPH);

	// Using FText because this is display text that should be localizable
	SpeedDisplayString = FText::Format(LOCTEXT("SpeedFormat", "{0} km/h"), FText::AsNumber(KPH_int));
	
	if (bInReverseGear == true)
	{
		GearDisplayString = FText(LOCTEXT("ReverseGear", "R"));
	}
	else
	{
		int32 Gear = GetVehicleMovement()->GetCurrentGear();
		GearDisplayString = (Gear == 0) ? LOCTEXT("N", "N") : FText::AsNumber(Gear);
	}	
}

void ALD32APGPawn::SetupInCarHUD()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if ((PlayerController != nullptr) && (InCarSpeed != nullptr) && (InCarGear != nullptr) )
	{
		// Setup the text render component strings
		InCarSpeed->SetText(SpeedDisplayString.ToString());
		InCarGear->SetText(GearDisplayString.ToString());
		
		if (bInReverseGear == false)
		{
			InCarGear->SetTextRenderColor(GearDisplayColor);
		}
		else
		{
			InCarGear->SetTextRenderColor(GearDisplayReverseColor);
		}
	}
}

#undef LOCTEXT_NAMESPACE
