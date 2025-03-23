// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/RepairRift/STMission_RepairRift.h"
#include "Net/UnrealNetwork.h"
#include "STGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Interact/Rift/InteractableRift.h"
#include "Actors/SpawnPoint/SpawnPoint_Rift.h"

USTMission_RepairRift::USTMission_RepairRift()
{
}

void USTMission_RepairRift::Tick(float DeltaTime)
{
	if (bIsMisionActivated)
	{
		CurrentMissionTime += DeltaTime;
		if (CurrentMissionTime > MaxMissionTime)
		{
			AActor* owner = GetTypedOuter<AActor>();
			if (owner->HasAuthority())
			{
				DeactivateMission(false);
			}
		}
	}
}

void USTMission_RepairRift::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USTMission_RepairRift, MaxMissionTime);
	DOREPLIFETIME(USTMission_RepairRift, RepairedRiftInfos);
}

void USTMission_RepairRift::ActivateMission()
{
	Super::ActivateMission();

	/*
		Set Mission
	*/
	bIsMisionActivated = true;
	CurrentMissionTime = 0;

	/*
		Spawn NPC On Random Point (Get All NPC Spawn Points From World)
	*/
	FVector SpawnLocation; FRotator SpawnRotation;

	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SubClassOfSpawnPoint, SpawnPoints);

	for (int i = 0; i < RepairedRiftInfos.Num(); i++)
	{
		// Set Random Point From SpawnPoints

		// SpawnLocation = 
		// SpawnRotation =

		if (RepairedRiftInfos[i].SubClassOfRift)
		{
			GetWorld()->SpawnActor<AActor>(RepairedRiftInfos[i].SubClassOfRift, SpawnLocation, SpawnRotation);
		}
	}


	/*
	*	Show Related Widget on Server & Clients
	*/
}

void USTMission_RepairRift::DeactivateMission(bool IsCleared)
{
	Super::DeactivateMission(IsCleared);

	bIsMisionActivated = false;

	/*
	*	Hide Related Widget On Server & Clients
	*/

	/*
		End Mission
	*/

	/*
		Ended Broadcast
	*/
	Delegate_MissionEnded.Broadcast(FSTGameplayTags::Get().Mission_RescueHostage, IsCleared);
}

bool USTMission_RepairRift::IsMissionCleared()
{
	for (int i = 0; i < RepairedRiftInfos.Num(); i++)
	{
		if (RepairedRiftInfos[i].bIsRepaired == false)
		{
			return false;
		}
	}

	return true;
}

void USTMission_RepairRift::UpdateRepairRiftInfo(int RiftID)
{
	for (int i = 0; i < RepairedRiftInfos.Num(); i++)
	{
		if (RepairedRiftInfos[i].RiftID == RiftID)
		{
			RepairedRiftInfos[i].bIsRepaired = true;
		}
	}

	if (IsMissionCleared())
	{
		DeactivateMission(true);
	}
}

void USTMission_RepairRift::OnRep_RepairedRiftInfos()
{
	// Update UI
}
