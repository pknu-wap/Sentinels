// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionCondition/KillCount/STKillCountCondition.h"
#include "Net/UnrealNetwork.h"

void USTKillCountCondition::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USTKillCountCondition, KillInfos);
}

bool USTKillCountCondition::IsSatisfied()
{
	for (int i = 0; i < KillInfos.Num(); i++)
	{
		if (KillInfos[i].Current != KillInfos[i].Required)
			return false;
	}

	return true;
}

void USTKillCountCondition::MissionActivated()
{
}

void USTKillCountCondition::MissionDeactivated(bool IsCleared)
{
}

void USTKillCountCondition::ConditionUpdated(int ObjectID, bool Success)
{
	for (int i = 0; i < KillInfos.Num(); i++)
	{
		if (KillInfos[i].MonsterID == ObjectID)
		{
			KillInfos[i].Current = FMath::Clamp(KillInfos[i].Current + 1, 0, KillInfos[i].Required);
		}
	}
}

void USTKillCountCondition::OnRep_KillInfos()
{
	UE_LOG(LogTemp, Display, TEXT("USTKillCountCondition::OnRep_KillInfos"));
}
