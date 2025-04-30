// Fill out your copyright notice in the Description page of Project Settings.


#include "System/STGameState.h"
#include "System/Mission/STMissionBase.h"
#include "Net/UnrealNetwork.h"
#include "Sentinels_LS.h"
#include "Kismet/KismetMathLibrary.h"

ASTGameState::ASTGameState() :
    CurrentLevelTag(FSTGameplayTags::Get().Level_Lobby)
{
    bReplicates = true;
    bReplicateUsingRegisteredSubObjectList = true;
}

void ASTGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASTGameState, Missions);
    DOREPLIFETIME(ASTGameState, ActivatedMission);
    DOREPLIFETIME(ASTGameState, SubMissions);
    DOREPLIFETIME(ASTGameState, CurrentLevelTag);
}

void ASTGameState::ActivateRandomMission()
{
    if (!HasAuthority())
    {
        ST_LOG(LogSTNetwork, Log, TEXT("ActivateMission should be called on server!"));
        return;
    }

    int rand = UKismetMathLibrary::RandomIntegerInRange(0, Missions.Num() - 1);
    if (Missions.IsValidIndex(rand))
    {
        ActivateMission(Missions[rand].MissionTag);
        ActivatedMission = Missions[rand];
    }
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

    for (int i = 0; i < Missions.Num(); i++)
    {
        if (Missions[i].MissionTag == InMissionTag)
        {
            ActivatedMission = Missions[i];
        }
    }

    OnRep_ActivatedMission();
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

void ASTGameState::RegisterRandomMissions(int RegisterMissionNum, const TArray<FRegisterMissionInfo>& AvailableMissionInfos)
{
    if (!HasAuthority())
    {
        ST_LOG(LogSTNetwork, Log, TEXT("RegisterMission should be called on server!"));
        return;
    }

    if (RegisterMissionNum > AvailableMissionInfos.Num())
    {
        UE_LOG(LogTemp, Display, TEXT("ASTGameState::RegisterRandomMissions Failed by Num of RegisterMission is bigger than Num Of AvailableMissionInfo ! "));
        return;
    }

    /*
        Register Random Mission
    */
    TSet<int> Registered;
    int registeredNum = 0; int rand;
    int MaxLoopIdx = 500; int CurrentLoopIdx = 0;
    while (registeredNum < RegisterMissionNum && CurrentLoopIdx < MaxLoopIdx)
    {
        rand = UKismetMathLibrary::RandomIntegerInRange(0, AvailableMissionInfos.Num() - 1);

        // Skip if mission is already registered
        if (Registered.Contains(rand))
            continue;

        // Register Mission
        if (AvailableMissionInfos[rand].MissionSubClass)
        {
            RegisterMission(AvailableMissionInfos[rand].MissionTag, AvailableMissionInfos[rand].MissionSubClass);
            Registered.Add(rand);
            registeredNum++;
        }

        CurrentLoopIdx++;
    }
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
        UE_LOG(LogTemp, Display, TEXT("%s is Already Registered Mission ! "), *mission->GetName());
        return;
    }

    USTMissionBase* NewMission = NewObject<USTMissionBase>(this, MissionSubClass);
    if (NewMission)
    {
        AddReplicatedSubObject(NewMission);
        Missions.Push(FMissionInfo(InMissionTag, NewMission));

        NewMission->RegisterMission();
        NewMission->Delegate_MissionEnded.AddDynamic(this, &ASTGameState::OnMissionEnded);
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
                // Automatically Destroyed by Garbage Collection
            }

            Missions.RemoveAt(i);
            return;
        }
    }

    for (int i = 0; i < SubMissions.Num(); i++)
    {
        if (SubMissions[i].MissionTag == InMissionTag)
        {
            if (SubMissions[i].Mission)
            {
                RemoveReplicatedSubObject(SubMissions[i].Mission);
                SubMissions[i].Mission = nullptr;

                // Should Destory Mission?
                // Automatically Destroyed by Garbage Collection
            }

            SubMissions.RemoveAt(i);
            return;
        }
    }
}

void ASTGameState::OnMissionEnded(FGameplayTag InMissionTag, bool IsCleared)
{
    // Mission Clear On Client
    OnMissionEnded_Multicast(InMissionTag, IsCleared);

    // UnRegister Mission On Server
    UnRegisterMission(InMissionTag);

    // Start Next Random Mission
    ActivateRandomMission();
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

void ASTGameState::RegisterSubMissions(const TArray<FRegisterMissionInfo>& InSubMissionInfos)
{
    if (!HasAuthority())
    {
        ST_LOG(LogSTNetwork, Log, TEXT("RegisterSubMissions should be called on server!"));
        return;
    }

    for (auto& SubMission : InSubMissionInfos)
    {
        RegisterSubMission(SubMission.MissionTag, SubMission.MissionSubClass);
    }
}

void ASTGameState::RegisterSubMission(FGameplayTag InMissionTag, TSubclassOf<USTMissionBase> SubMissionSubClass)
{
    if (!HasAuthority())
    {
        ST_LOG(LogSTNetwork, Log, TEXT("RegisterMission should be called on server!"));
        return;
    }

    USTMissionBase* submission = GetMission(InMissionTag);
    if (submission)
    {
        UE_LOG(LogTemp, Display, TEXT("%s is Already Registered SubMission ! "), *submission->GetName());
        return;
    }

    USTMissionBase* NewSubMission = NewObject<USTMissionBase>(this, SubMissionSubClass);
    if (NewSubMission)
    {
        AddReplicatedSubObject(NewSubMission);
        SubMissions.Push(FMissionInfo(InMissionTag, NewSubMission));

        NewSubMission->RegisterMission();
        NewSubMission->Delegate_MissionEnded.AddDynamic(this, &ASTGameState::OnSubMissionEnded);
    }
}

void ASTGameState::OnSubMissionEnded(FGameplayTag InMissionTag, bool IsCleared)
{
    // Mission Clear On Client
    OnSubMissionEnded_Multicast(InMissionTag, IsCleared);

    // UnRegister Mission On Server
    UnRegisterMission(InMissionTag);
}

void ASTGameState::OnSubMissionEnded_Multicast_Implementation(FGameplayTag InMissionTag, bool IsCleared)
{
    if (IsCleared)
    {
        ST_LOG(LogSTNetwork, Log, TEXT(" %s SubMission Cleared!"), *InMissionTag.GetTagName().ToString());
    }
    else
    {
        ST_LOG(LogSTNetwork, Log, TEXT(" %s SubMission Failed!"), *InMissionTag.GetTagName().ToString());
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

    for (int i = 0; i < SubMissions.Num(); i++)
    {
        if (SubMissions[i].MissionTag == InMissionTag)
        {
            return SubMissions[i].Mission;
        }
    }

    return nullptr;
}

bool ASTGameState::CanServerTravel()
{
    if (CurrentLevelTag == FSTGameplayTags::Get().Level_Lobby)
    {
        ST_LOG(LogSTNetwork, Log, TEXT("You must select map(level)!"));
        return false;
    }

    return true;
}

void ASTGameState::TryServerTravel()
{
    OnAllPlayerIsReady.Broadcast(CurrentLevelTag);
}

void ASTGameState::SetCurrentLevelTag(FGameplayTag NewLevelTag)
{
    CurrentLevelTag = NewLevelTag;
}

void ASTGameState::OnRep_ActivatedMission()
{
    Delegate_OnRepActivatedMission.Broadcast();
}

void ASTGameState::OnRep_SubMissions()
{
    Delegate_OnRepSubMissions.Broadcast();
}

