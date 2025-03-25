// Fill out your copyright notice in the Description page of Project Settings.


#include "System/STGameState.h"
#include "System/Mission/STMissionBase.h"
#include "Net/UnrealNetwork.h"
#include "Sentinels_LS.h"

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
    if (!HasAuthority())
    {
        ST_LOG(LogSTNetwork, Log, TEXT("ActivateMission should be called on server!"));
        return;
    }

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
    if (!HasAuthority())
    {
        ST_LOG(LogSTNetwork, Log, TEXT("RegisterMission should be called on server!"));
        return;
    }

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
    if (!HasAuthority())
    {
        ST_LOG(LogSTNetwork, Log, TEXT("UnRegisterMission should be called on server!"));
        return;
    }

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


    // UnRegister Mission On Server
    UnRegisterMission(InMissionTag);
}

void ASTGameState::OnMissionEnded_Multicast_Implementation(FGameplayTag InMissionTag, bool IsCleared)
{
    if (IsCleared)
    {
        ST_LOG(LogSTNetwork, Log, TEXT(" %s Mission Cleared!"), *InMissionTag.GetTagName().ToString());
    }
    else
    {
        ST_LOG(LogSTNetwork, Log, TEXT(" %s Mission Failed!"), *InMissionTag.GetTagName().ToString());
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

void ASTGameState::UpdateEliminatedMonsterInfo(int MonsterID)
{
    for (int i = 0; i < Missions.Num(); i++)
    {
        if (Missions[i].Mission)
        {
            Missions[i].Mission->UpdateEliminatedMonsterInfo_Server(MonsterID);
        }
    }
}

void ASTGameState::UpdateObjectDestroyedInfo(int ObjectID)
{
    for (int i = 0; i < Missions.Num(); i++)
    {
        if (Missions[i].Mission)
        {
            Missions[i].Mission->UpdateObjectDestroyedInfo_Server(ObjectID);
        }
    }
}

void ASTGameState::UpdateAcquiredQuestItemInfo(int ItemID)
{
    for (int i = 0; i < Missions.Num(); i++)
    {
        if (Missions[i].Mission)
        {
            Missions[i].Mission->UpdateAcquiredQuestItemInfo_Server(ItemID);
        }
    }
}

void ASTGameState::UpdateRescueHostageInfo(int NPCID)
{
    for (int i = 0; i < Missions.Num(); i++)
    {
        if (Missions[i].Mission)
        {
            Missions[i].Mission->UpdateRescueHostageInfo_Server(NPCID);
        }
    }
}

void ASTGameState::UpdateRepairRiftInfo(int RiftID)
{
    for (int i = 0; i < Missions.Num(); i++)
    {
        if (Missions[i].Mission)
        {
            Missions[i].Mission->UpdateRepairRiftInfo_Server(RiftID);
        }
    }
}
