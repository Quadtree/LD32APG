// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "LD32APG.h"
#include "LD32APGWheelFront.h"

ULD32APGWheelFront::ULD32APGWheelFront(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ShapeRadius = 35.f;
	ShapeWidth = 10.0f;
	bAffectedByHandbrake = false;
	SteerAngle = 50.f;
}
