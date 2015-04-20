// Fill out your copyright notice in the Description page of Project Settings.

#include "LD32APG.h"
#include "BuggyAIController.h"
#include "TeamStartZone.h"
#include "LD32APGPawn.h"
#include "VictoryPointComponent.h"
#include "WeaponConfiguration.h"

void ABuggyAIController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	ALD32APGPawn* pawn = Cast<ALD32APGPawn>(GetPawn());

	if (pawn)
	{
		if (StuckTime > 8 || pawn->GetActorUpVector().Z < 0)
		{
			pawn->SetJetPower(0);
			pawn->MoveRight(0);
			pawn->MoveForward(0);

			pawn->RevertToStartArea();

			return;
		}

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

		// make sure the gold hasn't been destroyed
		if (CurrentGoldTarget && CurrentGoldTarget->IsPendingKill()) CurrentGoldTarget = nullptr;

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
			//UE_LOG(LogTemp, Display, TEXT("Approaching homebase, towing %s"), *pawn->CurrentlyTowedGold->GetName());
			HeadTowards(myTeamStartZone->GetActorLocation());
		}
		else
		{
			if (CurrentGoldTarget)
			{
				//UE_LOG(LogTemp, Display, TEXT("Approaching %s"), *CurrentGoldTarget->GetName());
				HeadTowards(CurrentGoldTarget->GetActorLocation());

				if (IsCurrentlyBeingTowed(CurrentGoldTarget))
				{
					CurrentGoldTarget = nullptr;
				}

				if (CurrentGoldTarget && pawn->GetCurrentGrabTarget() == CurrentGoldTarget)
				{
					pawn->AttemptToGrabGold();
				}
			}
			else
			{
				//UE_LOG(LogTemp, Display, TEXT("Scanning for new gold"));

				int32 n = 0;

				// we need a gold target but we don't have one. so scan for some gold
				for (TActorIterator<AActor> itr(GetWorld()); itr; ++itr)
				{
					if (!goldOnTarget.Contains(*itr) && itr->FindComponentByClass<UVictoryPointComponent>() && !IsCurrentlyBeingTowed(*itr) && FMath::Rand() % ++n == 0)
					{
						// this gold isn't already in our base, so pick it as the target
						CurrentGoldTarget = *itr;
					}
				}
			}
		}

		if (pawn->GetVelocity().Size() > 400)
		{
			//UE_LOG(LogTemp, Display, TEXT("NOT STUCK %s"), *FString::SanitizeFloat(pawn->GetVelocity().Size()));
			StuckTime = 0;
		}
		else
		{
			//UE_LOG(LogTemp, Display, TEXT("STUCK %s"), *FString::SanitizeFloat(StuckTime));
			StuckTime += deltaTime;
		}

		if (StuckTime > 4)
		{
			pawn->SetJetPower(1);
		}

		pawn->StopFiring();

		for (TActorIterator<ALD32APGPawn> itr(GetWorld()); itr; ++itr)
		{
			if (itr->Team != pawn->Team && FVector::Dist(pawn->GetActorLocation(), itr->GetActorLocation()) < 2000)
			{
				pawn->StartFiring();
				break;
			}
		}
	}
}

void ABuggyAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	ALD32APGPawn* pawn = Cast<ALD32APGPawn>(InPawn);

	if (pawn)
	{
		pawn->WeaponConfiguration = NewObject<UWeaponConfiguration>();
		pawn->WeaponConfiguration->SetToRandom();
	}
}

bool ABuggyAIController::IsCurrentlyBeingTowed(AActor* actor)
{
	for (TActorIterator<ALD32APGPawn> itr(GetWorld()); itr; ++itr)
	{
		if (itr->CurrentlyTowedGold == actor)
		{
			return true;
		}
	}

	return false;
}

void ABuggyAIController::HeadTowards(FVector pos)
{
	ALD32APGPawn* pawn = Cast<ALD32APGPawn>(GetPawn());

	FVector facing = GetPawn()->GetActorRightVector();
	FVector toTarget = (pos - GetPawn()->GetActorLocation()).GetSafeNormal();

	float turn = FVector::DotProduct(toTarget, facing);

	if (turn < -0.2f)
	{
		pawn->MoveRight(-1);
		pawn->MoveForward(.7f);
	}
	else if (turn > .2f)
	{
		pawn->MoveRight(1);
		pawn->MoveForward(.7f);
	}
	else
	{
		pawn->MoveForward(1);
	}
}
