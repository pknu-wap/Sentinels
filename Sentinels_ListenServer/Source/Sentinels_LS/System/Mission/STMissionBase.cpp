// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/STMissionBase.h"
#include "Net/UnrealNetwork.h"
#include "MissionCondition/STMissionConditionBase.h"

void USTMissionBase::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Display, TEXT("USTMissionBase : Garbage Collected!"));
}

void USTMissionBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USTMissionBase, bIsMisionActivated);
	DOREPLIFETIME(USTMissionBase, MissionConditions);
}

void USTMissionBase::ActivateMission()
{
	UE_LOG(LogTemp, Display, TEXT("USTMissionBase::ActivateMission"));
	bIsMisionActivated = true;

	AActor* Owner = GetTypedOuter<AActor>();

	for (auto& conditionClass : SubclassOfMissionConditions)
	{
		USTMissionConditionBase* condition = NewObject<USTMissionConditionBase>(Owner, conditionClass);
		if (condition)
		{
			Owner->AddReplicatedSubObject(condition);
			MissionConditions.Push(condition);
		}
	}

	for (auto& condition : MissionConditions)
	{
		if(condition)
			condition->MissionActivated();
	}
}

void USTMissionBase::DeactivateMission(bool IsCleared)
{
	UE_LOG(LogTemp, Display, TEXT("USTMissionBase::DeactivateMission"));
	bIsMisionActivated = false;

	Delegate_MissionEnded.Broadcast(MissionTag, IsCleared);

	for (auto& condition : MissionConditions)
	{
		if (condition)
			condition->MissionDeactivated(IsCleared);
	}
}

bool USTMissionBase::IsMissionCleared()
{
	for (int i = 0; i < MissionConditions.Num(); i++)
	{
		if (MissionConditions[i] && !MissionConditions[i]->IsSatisfied())
		{
			return false;
		}
	}

	return true;
}

void USTMissionBase::UpdateEliminatedMonsterInfo_Server_Implementation(int MonsterID)
{
	for (int i = 0; i < MissionConditions.Num(); i++)
	{
		if (MissionConditions[i])
		{
			MissionConditions[i]->UpdateEliminatedMonsterInfo(MonsterID);
		}
	}

	if (IsMissionCleared())
	{
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [&]() {DeactivateMission(true); }, 0.5f, false);
		// GetWorld()->GetTimerManager().SetTimerForNextTick([&]() {DeactivateMission(true); });
	}
}

void USTMissionBase::UpdateObjectDestroyedInfo_Server_Implementation(int ObjectID)
{
	for (int i = 0; i < MissionConditions.Num(); i++)
	{
		if (MissionConditions[i])
		{
			MissionConditions[i]->UpdateObjectDestroyedInfo(ObjectID);
		}
	}

	if (IsMissionCleared())
	{
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [&]() {DeactivateMission(true); }, 0.5f, false);
		// GetWorld()->GetTimerManager().SetTimerForNextTick([&]() {DeactivateMission(true); });
	}
}

void USTMissionBase::UpdateAcquiredQuestItemInfo_Server_Implementation(int ItemID)
{
	for (int i = 0; i < MissionConditions.Num(); i++)
	{
		if (MissionConditions[i])
		{
			MissionConditions[i]->UpdateAcquiredQuestItemInfo(ItemID);
		}
	}

	if (IsMissionCleared())
	{
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [&]() {DeactivateMission(true); }, 0.5f, false);
		// GetWorld()->GetTimerManager().SetTimerForNextTick([&]() {DeactivateMission(true); });
	}
}

void USTMissionBase::UpdateRescueHostageInfo_Server_Implementation(int NPCID)
{
	for (int i = 0; i < MissionConditions.Num(); i++)
	{
		if (MissionConditions[i])
		{
			MissionConditions[i]->UpdateRescueHostageInfo(NPCID);
		}
	}

	if (IsMissionCleared())
	{
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [&]() {DeactivateMission(true); }, 0.5f, false);
		// GetWorld()->GetTimerManager().SetTimerForNextTick([&]() {DeactivateMission(true); });
	}
}

void USTMissionBase::UpdateRepairRiftInfo_Server_Implementation(int RiftID)
{
	for (int i = 0; i < MissionConditions.Num(); i++)
	{
		if (MissionConditions[i])
		{
			MissionConditions[i]->UpdateRepairRiftInfo(RiftID);
		}
	}

	if (IsMissionCleared())
	{
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [&]() {DeactivateMission(true); }, 0.5f, false);
		// GetWorld()->GetTimerManager().SetTimerForNextTick([&]() {DeactivateMission(true); });
	}
}

void USTMissionBase::OnRep_bIsMisionActivated()
{
	UE_LOG(LogTemp, Display, TEXT("USTMissionBase::OnRep_bIsMisionActivated"));
}
