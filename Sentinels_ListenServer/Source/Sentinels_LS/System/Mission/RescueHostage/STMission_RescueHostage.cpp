// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/RescueHostage/STMission_RescueHostage.h"
#include "Net/UnrealNetwork.h"
#include "STGameplayTags.h"

USTMission_RescueHostage::USTMission_RescueHostage()
{
}

void USTMission_RescueHostage::Tick(float DeltaTime)
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

void USTMission_RescueHostage::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USTMission_RescueHostage, MaxMissionTime);
	DOREPLIFETIME(USTMission_RescueHostage, RescuedNPCInfos);
}

void USTMission_RescueHostage::ActivateMission()
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

	// SpawnLocation = 
	// SpawnRotation =

	for (int i = 0; i < RescuedNPCInfos.Num(); i++)
	{
		if (RescuedNPCInfos[i].SubClassOfNPC)
		{
			GetWorld()->SpawnActor<AActor>(RescuedNPCInfos[i].SubClassOfNPC, SpawnLocation, SpawnRotation);
		}
	}


	/*
	*	Show Related Widget on Server & Clients
	*/
}

void USTMission_RescueHostage::DeactivateMission(bool IsCleared)
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

bool USTMission_RescueHostage::IsMissionCleared()
{
	for (int i = 0; i < RescuedNPCInfos.Num(); i++)
	{
		if (RescuedNPCInfos[i].bIsRescued == false)
		{
			return false;
		}
	}

	return true;
}

void USTMission_RescueHostage::UpdateRescueHostageInfo(int NPCID)
{
	for (int i = 0; i < RescuedNPCInfos.Num(); i++)
	{
		if (RescuedNPCInfos[i].NPCID == NPCID)
		{
			RescuedNPCInfos[i].bIsRescued = true;
		}
	}

	if (IsMissionCleared())
	{
		DeactivateMission(true);
	}
}

void USTMission_RescueHostage::OnRep_RescuedNPCInfos()
{
	// Update UI
}
