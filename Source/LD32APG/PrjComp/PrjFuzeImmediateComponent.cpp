// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "PrjFuzeImmediateComponent.h"

UPrjFuzeImmediateComponent::UPrjFuzeImmediateComponent()
{
	CostModifier = -.5f;
	Category = EPrjComponentCategory::PCC_Fuze;
	Duration = 0.01f;
}


