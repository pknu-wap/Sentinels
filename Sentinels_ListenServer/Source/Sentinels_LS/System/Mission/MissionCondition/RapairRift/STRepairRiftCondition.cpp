// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionCondition/RapairRift/STRepairRiftCondition.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/SpawnPoint/SpawnPointBase.h"
#include "Actors/MissionObject/Interactable/Rift/Rift.h"
#include "STGameplayTags.h"
#include "System/STGameState.h"
#include "System/Mission/STMissionBase.h"

void USTRepairRiftCondition::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USTRepairRiftCondition, RiftInfos)
}

bool USTRepairRiftCondition::IsSatisfied_Implementation()
{
	for (int i = 0; i < RiftInfos.Num(); i++)
	{
		if (RiftInfos[i].bIsRepaired == false)
		{
			return false;
		}
	}

	return true;
}

void USTRepairRiftCondition::MissionRegistered_Implementation()
{
	/*
		Spawn Rift On Random Point (Get All Rift Spawn Points From World)
	*/
	FVector SpawnLocation; FRotator SpawnRotation;

	TArray<ASpawnPointBase*> SpawnPoints;
	GetAllSpawnPointsWithTag(FSTGameplayTags::Get().Mission_RepairRift, SpawnPoints);

	if (SpawnPoints.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("USTMission_RepairRift::ActivateMission Failed to get SpawnPoints of NPC!"));
		return;
	}

	if (SpawnPoints.Num() < RiftInfos.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("USTMission_RepairRift::ActivateMission SpawnPoints Num is Smaller than Repair Rift Num."));
		return;
	}

	TSet<int> UsedSpawnPoints;
	for (int i = 0; i < RiftInfos.Num(); i++)
	{
		// Set Random Point From SpawnPoints

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

		if (RiftInfos[i].SubClassOfRift)
		{
			ARift* rift = GetWorld()->SpawnActor<ARift>(RiftInfos[i].SubClassOfRift, SpawnLocation, SpawnRotation);
			if (rift)
			{
				rift->SetObjectID(RiftInfos[i].RiftID);
				rift->Delegate_MissionConditionUpdate.AddUObject(this, &USTRepairRiftCondition::ConditionUpdated);
				SpawnedRifts.Push(rift);
			}
		}
	}
}

void USTRepairRiftCondition::MissionActivated_Implementation()
{
	/*
		Set Mission
	*/
	CurrentMissionTime = 0;

	/*
		Spawn Initial Enemy
	*/
	for (int i = 0; i < SpawnedRifts.Num(); i++)
	{
		if (SpawnedRifts[i])
		{
			SpawnedRifts[i]->SpawnInitialEnemy();
		}
	}

	/*
	*	Show Related Widget on Server & Clients
	*/
}

void USTRepairRiftCondition::MissionDeactivated_Implementation(bool IsCleared)
{
}

void USTRepairRiftCondition::ConditionUpdated(int ObjectID, bool Success)
{
	for (int i = 0; i < RiftInfos.Num(); i++)
	{
		if (RiftInfos[i].RiftID == ObjectID)
		{
			RiftInfos[i].bIsRepaired = Success;
		}
	}

	if (IsSatisfied())
	{
		if (Mission)
		{
			Mission->CheckMissionClearable();
		}
	}

	OnRep_RiftInfos();
}


void USTRepairRiftCondition::OnRep_RiftInfos()
{
	UE_LOG(LogTemp, Display, TEXT("s"));
	Delegate_ConditionUpdated.Broadcast();
}
