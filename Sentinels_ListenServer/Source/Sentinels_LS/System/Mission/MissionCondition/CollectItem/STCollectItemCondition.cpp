// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionCondition/CollectItem/STCollectItemCondition.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/SpawnPoint/SpawnPointBase.h"
#include "Actors/Spawner/Spawner.h"
#include "STGameplayTags.h"

void USTCollectItemCondition::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USTCollectItemCondition, CollectedItemInfos);
}

bool USTCollectItemCondition::IsSatisfied()
{
	for (int i = 0; i < CollectedItemInfos.Num(); i++)
	{
		if (CollectedItemInfos[i].Current != CollectedItemInfos[i].Required)
		{
			return false;
		}
	}

	return true;
}

void USTCollectItemCondition::MissionActivated()
{
	/*
		Set Mission
	*/
	CurrentMissionTime = 0;

	/*
		Spawn NPC On Random Point (Get All NPC Spawn Points From World)
	*/
	FVector SpawnLocation; FRotator SpawnRotation;

	TArray<ASpawnPointBase*> SpawnPoints;
	GetAllSpawnPointsWithTag(FSTGameplayTags::Get().SpawnPoint_Spawner, SpawnPoints);

	if (SpawnPoints.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("USTCollectItemCondition::MissionActivated Failed to get SpawnPoints of Spawner!"));
		return;
	}

	if (SpawnPoints.Num() < SpawnerInfos.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("USTCollectItemCondition::MissionActivated SpawnPoints Num is Smaller than Spawner Num."));
		return;
	}

	TSet<int> UsedSpawnPoints;
	for (int i = 0; i < SpawnerInfos.Num(); i++)
	{
		for (auto& actor : SpawnPoints)
		{
			ASpawnPointBase* SpawnerPoint = Cast<ASpawnPointBase>(actor);
			if (SpawnerPoint && SpawnerPoint->SpawnPointIdx == SpawnerInfos[i].SpawnPointIdx)
			{
				SpawnLocation = SpawnerPoint->GetActorLocation();
				SpawnRotation = SpawnerPoint->GetActorRotation();

				if (SpawnerInfos[i].SpawnerClass)
				{
					GetWorld()->SpawnActor<AActor>(SpawnerInfos[i].SpawnerClass, SpawnLocation, SpawnRotation);
				}

				break;
			}
		}
	}
}

void USTCollectItemCondition::MissionDeactivated(bool IsCleared)
{
}

void USTCollectItemCondition::OnRep_CollectedItemInfos()
{
	UE_LOG(LogTemp, Display, TEXT("USTCollectItemCondition::OnRep_CollectedItemInfos"));
	Delegate_ConditionUpdated.Broadcast();
}
