// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "LD32APG.h"
#include "LD32APGWheelRear.h"

ULD32APGWheelRear::ULD32APGWheelRear(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ShapeRadius = 35.f;
	ShapeWidth = 10.0f;
	bAffectedByHandbrake = true;
	SteerAngle = 0.f;
}
