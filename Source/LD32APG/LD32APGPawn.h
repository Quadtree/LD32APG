// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/WheeledVehicle.h"
#include "LD32APGPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UTextRenderComponent;
class UInputComponent;
UCLASS(config=Game)
class ALD32APGPawn : public AWheeledVehicle
{
	GENERATED_BODY()

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

public:
	ALD32APGPawn(const FObjectInitializer& ObjectInitializer);

	// Begin Pawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End Pawn interface

	// Begin Actor interface
	virtual void Tick(float Delta) override;
	virtual void BeginPlay() override;
	// End Actor interface

	/** Handle pressing forwards */
	void MoveForward(float Val);

	/** Update the physics material used by the vehicle mesh */
	void UpdatePhysicsMaterial();
	/** Handle pressing right */
	void MoveRight(float Val);

	static const FName LookUpBinding;
	static const FName LookRightBinding;

	UPROPERTY(BlueprintReadWrite, Category = WeaponConfiguration)
	class UWeaponConfiguration* WeaponConfiguration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Energy)
	float CurrentEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Energy)
	float MaxEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Energy)
	float EnergyRegeneratedPerSecond;


private:
	/* Are we on a 'slippery' surface */
	bool bIsLowFriction;


public:
	/** Returns SpringArm subobject **/
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
};
