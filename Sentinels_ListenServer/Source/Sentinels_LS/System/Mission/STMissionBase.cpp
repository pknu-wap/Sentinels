// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/STMissionBase.h"
#include "Net/UnrealNetwork.h"
#include "MissionCondition/STMissionConditionBase.h"

USTMissionBase::USTMissionBase()
{
	ProgressState = EMissionProgressState::None;
}

void USTMissionBase::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Display, TEXT("USTMissionBase : Garbage Collected!"));
}

void USTMissionBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass());
	if (BPClass != NULL)
	{
		BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
	}

	DOREPLIFETIME(USTMissionBase, MissionConditions);
	DOREPLIFETIME(USTMissionBase, ProgressState);
}

void USTMissionBase::RegisterMission()
{
	if (ProgressState != EMissionProgressState::None)
		return;

	UE_LOG(LogTemp, Display, TEXT("USTMissionBase::RegisterMission %s"), *MissionTag.GetTagName().ToString());
	AActor* Owner = GetTypedOuter<AActor>();
	for (auto& conditionClass : SubclassOfMissionConditions)
	{
		USTMissionConditionBase* condition = NewObject<USTMissionConditionBase>(Owner, conditionClass);
		if (condition)
		{
			condition->InitializeCondition(this, MissionTag);
			Owner->AddReplicatedSubObject(condition);
			MissionConditions.Push(condition);
			condition->MissionRegistered();
		}
	}

	ProgressState = EMissionProgressState::Registered;
	OnRep_ProgressState();

	// Execute On Blueprint
	OnMissionRegistered();
}

void USTMissionBase::ActivateMission()
{
	if (ProgressState != EMissionProgressState::Registered)
		return;

	UE_LOG(LogTemp, Display, TEXT("USTMissionBase::ActivateMission : %s is Activated!"), *GetName());

	AActor* Owner = GetTypedOuter<AActor>();

	for (auto& condition : MissionConditions)
	{
		if(condition)
			condition->MissionActivated();
	}

	ProgressState = EMissionProgressState::Activated;
	OnRep_ProgressState();

	// Execute On Blueprint
	OnMissionActivated();
}

void USTMissionBase::DeactivateMission(bool IsCleared)
{
	UE_LOG(LogTemp, Display, TEXT("USTMissionBase::DeactivateMission"));

	if (ProgressState != EMissionProgressState::Activated) return;

	MissionEnded_Multicast(IsCleared);

	for (auto& condition : MissionConditions)
	{
		if (condition)
			condition->MissionDeactivated(IsCleared);
	}

	ProgressState = IsCleared ? EMissionProgressState::Cleared : EMissionProgressState::Failed;
	OnRep_ProgressState();

	// Execute On Blueprint
	OnMissionDeactivated(IsCleared);
}

void USTMissionBase::MissionEnded_Multicast_Implementation(bool IsCleared)
{
	Delegate_MissionEnded.Broadcast(this, IsCleared);
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

void USTMissionBase::OnRep_ProgressState()
{
	UE_LOG(LogTemp, Display, TEXT("OnRep_ProgressState"));
	Delegate_MissionStateChanged.Broadcast(this, ProgressState);
}
