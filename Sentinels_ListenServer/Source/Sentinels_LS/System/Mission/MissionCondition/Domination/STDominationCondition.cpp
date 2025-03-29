// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionCondition/Domination/STDominationCondition.h"
#include "System/STGameState.h"
#include "System/Mission/STMissionBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/SpawnPoint/SpawnPointBase.h"
#include "Actors/MissionObject/NonInteractable/DominationPoint/DominationPoint.h"

void USTDominationCondition::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

bool USTDominationCondition::IsSatisfied()
{
	for (int i = 0; i < DominationPointInfos.Num(); i++)
	{
		if (DominationPointInfos[i].bIsDominated == false)
		{
			return false;
		}
	}

	return true;
}

void USTDominationCondition::MissionActivated()
{
	// Set Time Limit
	GetWorld()->GetTimerManager().SetTimer(
		handle, this, &USTDominationCondition::TimeLimitEnded, DominationTimeLimit, false);

	// Spawn Domination Core
	FVector SpawnLocation; FRotator SpawnRotation;

	TArray<ASpawnPointBase*> SpawnPoints;
	GetAllSpawnPointsWithTag(FSTGameplayTags::Get().SpawnPoint_DominationPoint, SpawnPoints);

	if (SpawnPoints.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("USTDominationCondition::MissionActivated Failed to get SpawnPoints of DominationPoint!"));
		return;
	}

	if (SpawnPoints.Num() < DominationPointInfos.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("USTDominationCondition::ActivateMission SpawnPoints Num is Smaller than DominationPointInfos."));
		return;
	}


	TSet<int> UsedSpawnPoints;
	for (int i = 0; i < DominationPointInfos.Num(); i++)
	{
		int Rand = -1; 

		while (true)
		{
			Rand = UKismetMathLibrary::RandomIntegerInRange(0, SpawnPoints.Num() - 1);
			if (!UsedSpawnPoints.Contains(Rand))
			{
				UsedSpawnPoints.Add(Rand);
				break;
			}
		}

		if (!SpawnPoints.IsValidIndex(Rand))
		{
			continue;
		}

		SpawnLocation = SpawnPoints[Rand]->GetActorLocation();
		SpawnRotation = SpawnPoints[Rand]->GetActorRotation();

		ADominationPoint* DP = GetWorld()->SpawnActor<ADominationPoint>(DominationPointInfos[i].SubClassOfDominationPoint, SpawnLocation, SpawnRotation);
		if (DP)
		{
			DP->SetObjectID(DominationPointInfos[i].DominationPID);
			DP->Delegate_MissionConditionUpdate.AddUObject(this, &USTDominationCondition::ConditionUpdated);
		}
	}
}

void USTDominationCondition::MissionDeactivated(bool IsCleared)
{

}

void USTDominationCondition::ConditionUpdated(int ObjectID, bool Success)
{
	for (int i = 0; i < DominationPointInfos.Num(); i++)
	{
		if (DominationPointInfos[i].DominationPID == ObjectID)
		{
			DominationPointInfos[i].bIsDominated = Success;
		}
	}

	if (IsSatisfied())
	{
		ASTGameState* GameState = Cast<ASTGameState>(GetWorld()->GetGameState());
		if (GameState)
		{
			USTMissionBase* Mission = GameState->GetMission(MissionTag);
			if (Mission)
			{
				Mission->CheckMissionClearable();
			}
		}
	}
}

void USTDominationCondition::TimeLimitEnded()
{
	// Time Limit Success
	ASTGameState* GameState = Cast<ASTGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		USTMissionBase* Mission = GameState->GetMission(MissionTag);
		if (Mission)
		{
			Mission->DeactivateMission(false);
		}
	}
}
