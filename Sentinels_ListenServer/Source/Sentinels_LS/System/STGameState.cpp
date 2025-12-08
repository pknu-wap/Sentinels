// Fill out your copyright notice in the Description page of Project Settings.


#include "System/STGameState.h"
#include "System/Mission/STMissionBase.h"
#include "System/STGameMode_MissionBase.h"
#include "Net/UnrealNetwork.h"
#include "Sentinels_LS.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/Section/STMissionSection.h"
#include "Kismet/GameplayStatics.h"
#include "SubSystem/STGameTravelDataSubsystem.h"

ASTGameState::ASTGameState() :
    LevelTag(FSTGameplayTags::Get().Level_Lobby)
{
    bReplicates = true;
    bReplicateUsingRegisteredSubObjectList = true;
}

void ASTGameState::BeginPlay()
{
    Super::BeginPlay();

    USTGameTravelDataSubsystem* gameTravelDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USTGameTravelDataSubsystem>();
    LevelTag = gameTravelDataSubsystem->GetCurrentLevelTag();
}

void ASTGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASTGameState, Missions);
    DOREPLIFETIME(ASTGameState, ActivatedMissions);
    DOREPLIFETIME(ASTGameState, SubMissions);
    DOREPLIFETIME(ASTGameState, LevelTag);
}

int ASTGameState::GetClearedMissionNum() const
{
    int count = 0;

    for(auto& mission : Missions)
    {
        if (mission.Mission && mission.Mission->GetProgressState() == EMissionProgressState::Cleared)
            count++;
    }

    return count;
}

ASTMissionSection* ASTGameState::GetActivatedMissionSection() const
{
    for (const auto& missionInfo : ActivatedMissions)
    {
        if (missionInfo.Mission && missionInfo.Mission->GetProgressState() == EMissionProgressState::Activated)
        {
            return missionInfo.MissionSection;
        }
    }

    return nullptr;
}

void ASTGameState::ActivateRandomMission()
{
    if (!HasAuthority())
    {
        ST_LOG(LogSTNetwork, Log, TEXT("ActivateMission should be called on server!"));
        return;
    }

    int count = 50;
    while (count--)
    {
        int rand = UKismetMathLibrary::RandomIntegerInRange(0, Missions.Num() - 1);

        if (Missions.IsValidIndex(rand) 
            && Missions[rand].Mission 
            && Missions[rand].Mission->GetProgressState() == EMissionProgressState::Registered)
        {
            ActivateMission(Missions[rand].MissionSection);

            break;
        }
    }
}

void ASTGameState::ActivateSubMission(FGameplayTag InMissionTag)
{
    if (!HasAuthority())
    {
        ST_LOG(LogSTNetwork, Log, TEXT("ActivateMission should be called on server!"));
        return;
    }

    USTMissionBase* mission = GetSubMission(InMissionTag);
    if (mission)
    {
        mission->ActivateMission();
    }
}

void ASTGameState::ActivateMission(ASTMissionSection* InMissionSection)
{
    for (int i = 0; i < Missions.Num(); i++)
    {
        if (Missions[i].MissionSection == InMissionSection && Missions[i].Mission)
        {
            if(Missions[i].Mission)
                Missions[i].Mission->ActivateMission();

            if (Missions[i].MissionSection)
                Missions[i].MissionSection->MissionActivated();

            ActivatedMissions.Add(Missions[i]);

            OnRep_ActivatedMission();

            return;
        }
    }
}

void ASTGameState::RegisterRandomMissions(int RegisterMissionNum, const TArray<FRegisterMissionInfo>& AvailableMissionInfos, ASTMissionSection* MissionSection)
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
            RegisterMission(AvailableMissionInfos[rand].MissionTag, AvailableMissionInfos[rand].MissionSubClass, MissionSection);
            Registered.Add(rand);
            registeredNum++;
        }

        CurrentLoopIdx++;
    }
}

void ASTGameState::RegisterMission(FGameplayTag InMissionTag, TSubclassOf<USTMissionBase> MissionSubClass, ASTMissionSection* MissionSection)
{
    if (!HasAuthority())
    {
        ST_LOG(LogSTNetwork, Log, TEXT("RegisterMission should be called on server!"));
        return;
    }

    USTMissionBase* NewMission = NewObject<USTMissionBase>(this, MissionSubClass);
    if (NewMission)
    {
        AddReplicatedSubObject(NewMission);
        Missions.Push(FMissionInfo(NewMission, InMissionTag, MissionSection));

        NewMission->RegisterMission();
        NewMission->Delegate_MissionEnded.AddDynamic(this, &ASTGameState::OnMissionEnded);

        ASTGameMode_MissionBase* STGameMode = Cast<ASTGameMode_MissionBase>(UGameplayStatics::GetGameMode(this));
        if (STGameMode)
        {
            NewMission->Delegate_MissionEnded.AddDynamic(STGameMode, &ASTGameMode_MissionBase::OnMissionEnded);
        }

        OnRegisterMission.Broadcast();
    }
}

void ASTGameState::OnMissionEnded(USTMissionBase* InMission, bool IsCleared)
{
    // Mission Clear On Client
    OnMissionEnded_Multicast(InMission, IsCleared);

    for (int i = 0; i < Missions.Num(); i++)
    {
        if (Missions[i].Mission == InMission)
        {
            if (Missions[i].MissionSection)
                Missions[i].MissionSection->MissionDeactivated();
            break;
        }
    }

    // Start Next Random Mission
    // ActivateRandomMission();
}

void ASTGameState::OnMissionEnded_Multicast_Implementation(USTMissionBase* InMission, bool IsCleared)
{
    if (InMission)
    {
        if (IsCleared)
        {
            ST_LOG(LogSTNetwork, Log, TEXT(" %s Mission Cleared!"), *InMission->GetName());
        }
        else
        {
            ST_LOG(LogSTNetwork, Log, TEXT(" %s Mission Failed!"), *InMission->GetName());
        }
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

    OnRep_SubMissions();
}

void ASTGameState::RegisterSubMission(FGameplayTag InMissionTag, TSubclassOf<USTMissionBase> SubMissionSubClass)
{
    if (!HasAuthority())
    {
        ST_LOG(LogSTNetwork, Log, TEXT("RegisterMission should be called on server!"));
        return;
    }

    USTMissionBase* submission = GetSubMission(InMissionTag);
    if (submission)
    {
        UE_LOG(LogTemp, Display, TEXT("%s is Already Registered SubMission ! "), *submission->GetName());
        return;
    }

    USTMissionBase* NewSubMission = NewObject<USTMissionBase>(this, SubMissionSubClass);
    if (NewSubMission)
    {
        AddReplicatedSubObject(NewSubMission);
        SubMissions.Push(FSubMissionInfo(NewSubMission, InMissionTag));

        NewSubMission->RegisterMission();
        NewSubMission->Delegate_MissionEnded.AddDynamic(this, &ASTGameState::OnSubMissionEnded);
    }
}

void ASTGameState::OnSubMissionEnded(USTMissionBase* InMission, bool IsCleared)
{
    // Mission Clear On Client
    OnSubMissionEnded_Multicast(InMission, IsCleared);
}

void ASTGameState::OnSubMissionEnded_Multicast_Implementation(USTMissionBase* InMission, bool IsCleared)
{
    if (InMission)
    {
        if (IsCleared)
        {
            ST_LOG(LogSTNetwork, Log, TEXT(" %s SubMission Cleared!"), *InMission->GetName());
        }
        else
        {
            ST_LOG(LogSTNetwork, Log, TEXT(" %s SubMission Failed!"), *InMission->GetName());
        }
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

USTMissionBase* ASTGameState::GetSubMission(FGameplayTag InMissionTag)
{
    for (int i = 0; i < SubMissions.Num(); i++)
    {
        if (SubMissions[i].MissionTag == InMissionTag)
        {
            return SubMissions[i].Mission;
        }
    }

    return nullptr;
}

void ASTGameState::TryServerTravel()
{
    OnServerTravelReady.Broadcast(LevelTag);
}

void ASTGameState::SetLevelTag(FGameplayTag NewLevelTag)
{
    LevelTag = NewLevelTag;
}

void ASTGameState::OnRep_ActivatedMission()
{
    Delegate_OnRepActivatedMission.Broadcast();
}

void ASTGameState::OnRep_SubMissions()
{
    Delegate_OnRepSubMissions.Broadcast();
}

