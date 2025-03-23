// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/STMissionBase.h"
#include "Net/UnrealNetwork.h"

void USTMissionBase::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Display, TEXT("USTMissionBase : Garbage Collected!"));
}

void USTMissionBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USTMissionBase, bIsMisionActivated);
}

void USTMissionBase::ActivateMission()
{
	UE_LOG(LogTemp, Display, TEXT("USTMissionBase::ActivateMission"));
	bIsMisionActivated = true;
}

void USTMissionBase::DeactivateMission(bool IsCleared)
{
	UE_LOG(LogTemp, Display, TEXT("USTMissionBase::DeactivateMission"));
	bIsMisionActivated = false;
}

void USTMissionBase::UpdateEliminatedMonsterInfo_Server_Implementation(int MonsterID)
{
}

void USTMissionBase::UpdateObjectDestroyedInfo_Server_Implementation(int ObjectID)
{
}

void USTMissionBase::UpdateAcquiredQuestItemInfo_Server_Implementation(int ItemID)
{
}

void USTMissionBase::UpdateRescueHostageInfo_Server_Implementation(int NPCID)
{
}

void USTMissionBase::UpdateRepairRiftInfo_Server_Implementation(int RiftID)
{
}

void USTMissionBase::OnRep_bIsMisionActivated()
{
	UE_LOG(LogTemp, Display, TEXT("USTMissionBase::OnRep_bIsMisionActivated"));
	if (bIsMisionActivated)
	{
		ActivateMission();
	}
}
