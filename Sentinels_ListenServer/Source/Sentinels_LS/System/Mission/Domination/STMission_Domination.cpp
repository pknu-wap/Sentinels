// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/Domination/STMission_Domination.h"
#include "Net/UnrealNetwork.h"
#include "STGameplayTags.h"

USTMission_Domination::USTMission_Domination()
{
}

void USTMission_Domination::Tick(float DeltaTime)
{
	if (bIsMisionActivated)
	{
		CurrentMissionTime += DeltaTime;
		if (CurrentMissionTime > MaxMissionTime)
		{
			DeactivateMission(false);
		}
	}
}

void USTMission_Domination::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USTMission_Domination, MaxMissionTime);
	DOREPLIFETIME(USTMission_Domination, MonsterInfos);
}

void USTMission_Domination::ActivateMission()
{
	Super::ActivateMission();

	/*
		Set Mission
	*/
	bIsMisionActivated = true;
	CurrentMissionTime = 0;

	/*
	*	Show Related Widget on Server & Clients
	*/
}

void USTMission_Domination::DeactivateMission(bool IsCleared)
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
	Delegate_MissionEnded.Broadcast(FSTGameplayTags::Get().Mission_Domination, IsCleared);
}

bool USTMission_Domination::IsMissionCleared()
{
	for (int i = 0; i < MonsterInfos.Num(); i++)
	{
		if (MonsterInfos[i].Current != MonsterInfos[i].Required)
			return false;
	}

	return true;
}

void USTMission_Domination::UpdateEliminatedMonsterInfo(int MonsterID)
{
	for (int i = 0; i < MonsterInfos.Num(); i++)
	{
		if (MonsterInfos[i].MonsterID == MonsterID)
		{
			MonsterInfos[i].Current = FMath::Clamp(MonsterInfos[i].Current + 1, 0, MonsterInfos[i].Required);
		}
	}

	if (IsMissionCleared())
	{
		DeactivateMission(true);
	}
}
