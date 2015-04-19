// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "PrjAttractionExplosionComponent.h"

UPrjAttractionExplosionComponent::UPrjAttractionExplosionComponent()
{
	CostModifier = 2;
	Category = EPrjComponentCategory::PCC_Warhead;
	Radius = 5000;
	Strength = -800000;
}


