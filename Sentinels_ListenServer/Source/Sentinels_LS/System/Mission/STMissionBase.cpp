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

void USTMissionBase::RegisterMission()
{
	UE_LOG(LogTemp, Display, TEXT("USTMissionBase::RegisterMission %s"), *MissionTag.GetTagName().ToString());
	AActor* Owner = GetTypedOuter<AActor>();
	for (auto& conditionClass : SubclassOfMissionConditions)
	{
		USTMissionConditionBase* condition = NewObject<USTMissionConditionBase>(Owner, conditionClass);
		if (condition)
		{
			condition->SetMissionTag(MissionTag);
			Owner->AddReplicatedSubObject(condition);
			MissionConditions.Push(condition);
			condition->MissionRegistered();
		}
	}
}

void USTMissionBase::ActivateMission()
{
	if (bIsMisionActivated)
		return;

	UE_LOG(LogTemp, Display, TEXT("USTMissionBase::ActivateMission : %s is Activated!"), *GetName());
	bIsMisionActivated = true;

	AActor* Owner = GetTypedOuter<AActor>();

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

void USTMissionBase::CheckMissionClearable()
{
	for (int i = 0; i < MissionConditions.Num(); i++)
	{
		if (MissionConditions[i] && !MissionConditions[i]->IsSatisfied())
		{
			return;
		}
	}

	DeactivateMission(true);
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

void USTMissionBase::OnRep_bIsMisionActivated()
{
	UE_LOG(LogTemp, Display, TEXT("USTMissionBase::OnRep_bIsMisionActivated"));
}
