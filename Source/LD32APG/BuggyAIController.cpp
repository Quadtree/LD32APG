// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "BuggyAIController.h"
#include "TeamStartZone.h"
#include "LD32APGPawn.h"
#include "VictoryPointComponent.h"

void ABuggyAIController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	ALD32APGPawn* pawn = Cast<ALD32APGPawn>(GetPawn());

	if (pawn)
	{
		ATeamStartZone* myTeamStartZone = nullptr;

		int32 myTeam = pawn->Team;

		for (TActorIterator<ATeamStartZone> itr(GetWorld()); itr; ++itr)
		{
			if (itr->Team == myTeam)
			{
				myTeamStartZone = *itr;
			}
		}

		TArray<AActor*> goldOnTarget;

		// determine what gold is already on our team target
		if (myTeamStartZone)
		{
			goldOnTarget = myTeamStartZone->GetGoldOnTarget();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to find my start zone %s"), *GetName());
		}

		// check to see if our current gold target is now in our target area
		if (CurrentGoldTarget && myTeamStartZone)
		{
			if (goldOnTarget.Contains(CurrentGoldTarget))
			{
				// this gold is already on the target, so remove it
				CurrentGoldTarget = nullptr;
			}
		}

		// if we're towing something that isn't our current target, drop it
		if (pawn->CurrentlyTowedGold && pawn->CurrentlyTowedGold != CurrentGoldTarget)
		{
			pawn->AttemptToGrabGold();
		}

		if (pawn->CurrentlyTowedGold)
		{
			HeadTowards(myTeamStartZone->GetActorLocation());
		}
		else
		{
			if (CurrentGoldTarget)
			{
				HeadTowards(CurrentGoldTarget->GetActorLocation());

				if (pawn->GetCurrentGrabTarget() == CurrentGoldTarget)
				{
					pawn->AttemptToGrabGold();
				}
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("Scanning for new gold"));
				// we need a gold target but we don't have one. so scan for some gold
				for (TActorIterator<AActor> itr(GetWorld()); itr; ++itr)
				{
					if (!goldOnTarget.Contains(*itr) && itr->FindComponentByClass<UVictoryPointComponent>())
					{
						// this gold isn't already in our base, so pick it as the target
						CurrentGoldTarget = *itr;
						break;
					}
				}
			}
		}
	}
}

void ABuggyAIController::HeadTowards(FVector pos)
{
	UE_LOG(LogTemp, Display, TEXT("Approaching %s"), *pos.ToCompactString());
}
