// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionCondition/Domination/STDominationCondition.h"
#include "System/STGameState.h"
#include "System/Mission/STMissionBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/SpawnPoint/SpawnPointBase.h"
#include "Actors/MissionObject/NonInteractable/DefenseCore/DefenseCore.h"

void USTDominationCondition::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

bool USTDominationCondition::IsSatisfied()
{
	return false;
}

void USTDominationCondition::MissionActivated()
{

}

void USTDominationCondition::MissionDeactivated(bool IsCleared)
{

}