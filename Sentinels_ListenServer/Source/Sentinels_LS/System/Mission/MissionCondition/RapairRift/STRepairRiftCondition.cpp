// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionCondition/RapairRift/STRepairRiftCondition.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/SpawnPoint/SpawnPoint_Rift.h"
#include "Actors/Interact/Rift/InteractableRift.h"

void USTRepairRiftCondition::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USTRepairRiftCondition, RiftInfos)
}

bool USTRepairRiftCondition::IsSatisfied()
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

void USTRepairRiftCondition::MissionActivated()
{
	/*
		Set Mission
	*/
	CurrentMissionTime = 0;

	/*
		Spawn NPC On Random Point (Get All NPC Spawn Points From World)
	*/
	FVector SpawnLocation; FRotator SpawnRotation;

	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SubClassOfSpawnPoint, SpawnPoints);

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
			GetWorld()->SpawnActor<AActor>(RiftInfos[i].SubClassOfRift, SpawnLocation, SpawnRotation);
		}
	}


	/*
	*	Show Related Widget on Server & Clients
	*/
}

void USTRepairRiftCondition::MissionDeactivated(bool IsCleared)
{
}

void USTRepairRiftCondition::UpdateRepairRiftInfo(int RiftID)
{
	for (int i = 0; i < RiftInfos.Num(); i++)
	{
		if (RiftInfos[i].RiftID == RiftID)
		{
			RiftInfos[i].bIsRepaired = true;
		}
	}
}

void USTRepairRiftCondition::OnRep_RiftInfos()
{
	UE_LOG(LogTemp, Display, TEXT("USTRepairRiftCondition::OnRep_RiftInfos"));
}
