// Fill out your copyright notice in the Description page of Project Settings.


#include "System/STGameState.h"
#include "System/Mission/STMissionBase.h"
#include "Net/UnrealNetwork.h"

void ASTGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASTGameState, Missions);
}

void ASTGameState::ActivateMission(FGameplayTag InMissionTag)
{
    ISTMissionBase* mission = GetMission(InMissionTag);
    if (mission)
    {
        mission->ActivateMission();
    }
}

void ASTGameState::DeactivateMission(FGameplayTag InMissionTag)
{
    ISTMissionBase* mission = GetMission(InMissionTag);
    if (mission)
    {
        mission->DeactivateMission();
    }
}

bool ASTGameState::IsMissionCleared(FGameplayTag InMissionTag)
{
    return false;
}

ISTMissionBase* ASTGameState::GetMission(FGameplayTag InMissionTag)
{
    for (int i = 0; i < Missions.Num(); i++)
    {
        if (Missions[i].MissionTag == InMissionTag)
        {
            return Missions[i].Mission;
        }
    }

    return nullptr;
}
