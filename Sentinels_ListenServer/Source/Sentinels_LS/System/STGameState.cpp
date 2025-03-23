// Fill out your copyright notice in the Description page of Project Settings.


#include "System/STGameState.h"
#include "System/Mission/STMissionBase.h"
#include "Net/UnrealNetwork.h"

ASTGameState::ASTGameState()
{
    bReplicates = true;
    bReplicateUsingRegisteredSubObjectList = true;
}

void ASTGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASTGameState, Missions);
}

void ASTGameState::ActivateMission(FGameplayTag InMissionTag)
{
    USTMissionBase* mission = GetMission(InMissionTag);
    if (mission)
    {
        mission->ActivateMission();
    }
}

bool ASTGameState::IsMissionCleared(FGameplayTag InMissionTag)
{
    USTMissionBase* mission = GetMission(InMissionTag);
    if (mission)
    {
        return mission->IsMissionCleared();
    }

    return false;
}

void ASTGameState::RegisterMission(FGameplayTag InMissionTag, TSubclassOf<USTMissionBase> MissionSubClass)
{
    USTMissionBase* mission = GetMission(InMissionTag);
    if (mission)
    {
        UnRegisterMission(InMissionTag);
    }

    USTMissionBase* NewMission = NewObject<USTMissionBase>(this, MissionSubClass);
    if (NewMission)
    {
        NewMission->Delegate_MissionEnded.AddDynamic(this, &ASTGameState::OnMissionEnded);

        AddReplicatedSubObject(NewMission);
        Missions.Push(FMissionInfo(InMissionTag, NewMission));
    }
}

void ASTGameState::UnRegisterMission(FGameplayTag InMissionTag)
{
    for (int i = 0; i < Missions.Num(); i++)
    {
        if (Missions[i].MissionTag == InMissionTag)
        {
            if (Missions[i].Mission)
            {
                RemoveReplicatedSubObject(Missions[i].Mission);
                Missions[i].Mission = nullptr;

                // Should Destory Mission?
            }

            Missions.RemoveAt(i);
            return;
        }
    }
}

void ASTGameState::OnMissionEnded(FGameplayTag InMissionTag, bool IsCleared)
{
    // Mission Clear On Client
    OnMissionEnded_Multicast(InMissionTag, IsCleared);

    // Start Next Mission 

    if (IsCleared)
    {
        UE_LOG(LogTemp, Display, TEXT("ASTGameState : %s Mission Cleared!"), *InMissionTag.GetTagName().ToString());
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("ASTGameState : %s Mission Failed!"), *InMissionTag.GetTagName().ToString());
    }

    // UnRegister Mission On Server
    UnRegisterMission(InMissionTag);
}

void ASTGameState::OnMissionEnded_Multicast_Implementation(FGameplayTag InMissionTag, bool IsCleared)
{
    if (IsCleared)
    {
        UE_LOG(LogTemp, Display, TEXT("ASTGameState_Client : %s Mission Cleared!"), *InMissionTag.GetTagName().ToString());
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("ASTGameState_Client : %s Mission Failed!"), *InMissionTag.GetTagName().ToString());
    }
}

USTMissionBase* ASTGameState::GetMission(FGameplayTag InMissionTag)
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