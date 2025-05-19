// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/STGameTravelDataSubsystem.h"

USTGameTravelDataSubsystem::USTGameTravelDataSubsystem() :
    CurrentLevelTag(FSTGameplayTags::Get().Level_Lobby)
{
}

void USTGameTravelDataSubsystem::RegisterPlayerInfo(FUniqueNetIdRepl PlayerID, FPlayerInfo& PlayerInfo)
{
    if (!PlayerInfos.Contains(PlayerID))
        PlayerInfos.Add({ PlayerID, PlayerInfo });
}

void USTGameTravelDataSubsystem::UnRegisterPlayerInfo(FUniqueNetIdRepl PlayerID)
{
    FPlayerInfo* playerInfo = PlayerInfos.Find(PlayerID);
    if (playerInfo)
        PlayerInfos.Remove(PlayerID);
}

FPlayerInfo USTGameTravelDataSubsystem::GetPlayerInfo(FUniqueNetIdRepl PlayerID)
{
    if (!PlayerInfos.Contains(PlayerID))
        return FPlayerInfo();

    return *PlayerInfos.Find(PlayerID);
}

void USTGameTravelDataSubsystem::ChangePlayerInfo(FUniqueNetIdRepl PlayerID, FPlayerInfo& PlayerInfo)
{
    if (!PlayerInfos.Contains(PlayerID))
        RegisterPlayerInfo(PlayerID, PlayerInfo);

    PlayerInfos.Find(PlayerID)->PlayerClass = PlayerInfo.PlayerClass;
}
