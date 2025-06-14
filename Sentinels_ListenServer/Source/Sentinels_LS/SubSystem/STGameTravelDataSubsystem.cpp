// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/STGameTravelDataSubsystem.h"

USTGameTravelDataSubsystem::USTGameTravelDataSubsystem() :
    CurrentLevelTag(FSTGameplayTags::Get().Level_Lobby)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_HEAD(TEXT("/Script/Engine.DataTable'/Game/Sentinels/Player/ETC/Customize/DataTable/DT_Head.DT_Head'"));
	if (DT_HEAD.Succeeded())
	{
		DT_Head = DT_HEAD.Object;
	}

    static ConstructorHelpers::FObjectFinder<UDataTable> DT_HOOD(TEXT("/Script/Engine.DataTable'/Game/Sentinels/Player/ETC/Customize/DataTable/DT_Hood.DT_Hood'"));
    if (DT_HOOD.Succeeded())
    {
        DT_Hood = DT_HOOD.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> DT_LONGHAIR(TEXT("/Script/Engine.DataTable'/Game/Sentinels/Player/ETC/Customize/DataTable/DT_LongHair.DT_LongHair'"));
    if (DT_LONGHAIR.Succeeded())
    {
        DT_LongHair = DT_LONGHAIR.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> DT_GLASSES(TEXT("/Script/Engine.DataTable'/Game/Sentinels/Player/ETC/Customize/DataTable/DT_Glasses.DT_Glasses'"));
    if (DT_GLASSES.Succeeded())
    {
        DT_Glasses = DT_GLASSES.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> DT_UPPERBODY(TEXT("/Script/Engine.DataTable'/Game/Sentinels/Player/ETC/Customize/DataTable/DT_UpperBody.DT_UpperBody'"));
    if (DT_UPPERBODY.Succeeded())
    {
        DT_UpperBody = DT_UPPERBODY.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> DT_BACKPACK(TEXT("/Script/Engine.DataTable'/Game/Sentinels/Player/ETC/Customize/DataTable/DT_Backpack.DT_Backpack'"));
    if (DT_BACKPACK.Succeeded())
    {
        DT_Backpack = DT_BACKPACK.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> DT_HAND_L(TEXT("/Script/Engine.DataTable'/Game/Sentinels/Player/ETC/Customize/DataTable/DT_Hand_L.DT_Hand_L'"));
    if (DT_HAND_L.Succeeded())
    {
        DT_Hand_L = DT_HAND_L.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> DT_HAND_R(TEXT("/Script/Engine.DataTable'/Game/Sentinels/Player/ETC/Customize/DataTable/DT_Hand_R.DT_Hand_R'"));
    if (DT_HAND_R.Succeeded())
    {
        DT_Hand_R = DT_HAND_R.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> DT_BOTTOMBODY(TEXT("/Script/Engine.DataTable'/Game/Sentinels/Player/ETC/Customize/DataTable/DT_BottomBody.DT_BottomBody'"));
    if (DT_BOTTOMBODY.Succeeded())
    {
        DT_BottomBody = DT_BOTTOMBODY.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> DT_FEET(TEXT("/Script/Engine.DataTable'/Game/Sentinels/Player/ETC/Customize/DataTable/DT_Feet.DT_Feet'"));
    if (DT_FEET.Succeeded())
    {
        DT_Feet = DT_FEET.Object;
    }
}

void USTGameTravelDataSubsystem::RegisterPlayerInfo(FUniqueNetIdRepl PlayerID, FPlayerInfo PlayerInfo)
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

const FPlayerInfo& USTGameTravelDataSubsystem::LoadPlayerInfo(FUniqueNetIdRepl PlayerID)
{
    if (!PlayerInfos.Contains(PlayerID))
    {
        UE_LOG(LogTemp, Warning, TEXT("return default playerinfo"));
        FPlayerInfo playerInfo;
        RegisterPlayerInfo(PlayerID, playerInfo);
    }
       
    return *PlayerInfos.Find(PlayerID);
}

void USTGameTravelDataSubsystem::SavePlayerCurrentClass(FUniqueNetIdRepl PlayerID, ESTClassType ClassType)
{
    if (!PlayerInfos.Contains(PlayerID))
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerInfos can't find"));
        return;
    }
        
    PlayerInfos.Find(PlayerID)->PlayerClass = ClassType;
}

const UDataTable* USTGameTravelDataSubsystem::GetSKMeshDT(ESKParts SKPart)
{
    switch (SKPart)
    {
    case ESKParts::Head:
        return DT_Head;
    case ESKParts::Hood:
        return DT_Hood;
    case ESKParts::LongHair:
        return DT_LongHair;
    case ESKParts::Glasses:
        return DT_Glasses;
    case ESKParts::UpperBody:
        return DT_UpperBody;
    case ESKParts::Backpack:
        return DT_Backpack;
    case ESKParts::Hand_L:
        return DT_Hand_L;
    case ESKParts::Hand_R:
        return DT_Hand_R;
    case ESKParts::BottomBody:
        return DT_BottomBody;
    case ESKParts::Feet:
        return DT_Feet;
    default:
        return DT_Head;
    }
}

void USTGameTravelDataSubsystem::SavePlayerSKMeshes(FUniqueNetIdRepl PlayerID, FPlayerSKMeshesRowName& SKMeshesRowName)
{
    if (!PlayerInfos.Contains(PlayerID))
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerInfos can't find"));
        return;
    }

    PlayerInfos.Find(PlayerID)->PlayerSKMeshesRowName = SKMeshesRowName;
}
