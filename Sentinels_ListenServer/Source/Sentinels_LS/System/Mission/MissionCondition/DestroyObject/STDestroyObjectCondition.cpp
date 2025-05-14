// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionCondition/DestroyObject/STDestroyObjectCondition.h"
#include "System/STGameState.h"
#include "System/Mission/STMissionBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/SpawnPoint/SpawnPointBase.h"
#include "Actors/MissionObject/NonInteractable/DestructibleObject/DestructibleObject.h"
#include "Net/UnrealNetwork.h"

void USTDestroyObjectCondition::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USTDestroyObjectCondition, DestructibleObjectInfos);
}

bool USTDestroyObjectCondition::IsSatisfied_Implementation()
{
	for (int i = 0; i < DestructibleObjectInfos.Num(); i++)
	{
		if (DestructibleObjectInfos[i].bIsDestroyed == false)
		{
			return false;
		}
	}

	return true;
}

void USTDestroyObjectCondition::MissionActivated_Implementation()
{
	// Set Time Limit
	GetWorld()->GetTimerManager().SetTimer(
		handle, this, &USTDestroyObjectCondition::TimeLimitEnded, TimeLimit, false);

	// Spawn Domination Core
	FVector SpawnLocation; FRotator SpawnRotation;

	TArray<ASpawnPointBase*> SpawnPoints;
	GetAllSpawnPointsWithTag(FSTGameplayTags::Get().Mission_DestroyObjects, SpawnPoints);

	if (SpawnPoints.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("USTDestroyObjectCondition::MissionActivated Failed to get SpawnPoints of DestructibleObject!"));
		return;
	}

	if (SpawnPoints.Num() < DestructibleObjectInfos.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("USTDestroyObjectCondition::ActivateMission SpawnPoints Num is Smaller than DestructibleObjectInfos."));
		return;
	}

	TSet<int> UsedSpawnPoints;
	for (int i = 0; i < DestructibleObjectInfos.Num(); i++)
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

		ADestructibleObject* object = GetWorld()->SpawnActor<ADestructibleObject>(DestructibleObjectInfos[i].SubClassOfDestructibleObject, SpawnLocation, SpawnRotation);
		if (object)
		{
			object->SetObjectID(DestructibleObjectInfos[i].ObjectID);
			object->Delegate_MissionConditionUpdate.AddUObject(this, &USTDestroyObjectCondition::ConditionUpdated);
		}
	}
}

void USTDestroyObjectCondition::MissionDeactivated_Implementation(bool IsCleared)
{
}

void USTDestroyObjectCondition::ConditionUpdated(int ObjectID, bool Success)
{
	for (int i = 0; i < DestructibleObjectInfos.Num(); i++)
	{
		if (DestructibleObjectInfos[i].ObjectID == ObjectID)
		{
			DestructibleObjectInfos[i].bIsDestroyed = Success;
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

	OnRep_DestructibleObjectInfos();
}

void USTDestroyObjectCondition::OnRep_DestructibleObjectInfos()
{
	Delegate_ConditionUpdated.Broadcast();
}

void USTDestroyObjectCondition::TimeLimitEnded()
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
