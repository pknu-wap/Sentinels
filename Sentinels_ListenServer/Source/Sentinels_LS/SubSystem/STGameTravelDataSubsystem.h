// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "STEnums.h"
#include "STGameplayTags.h"
#include "STGameTravelDataSubsystem.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	ESTClassType PlayerClass = ESTClassType::GreatSword;

	// 플레이어 메시 파츠
};

UCLASS()
class SENTINELS_LS_API USTGameTravelDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	USTGameTravelDataSubsystem();

public:
	UFUNCTION(BlueprintCallable)
	void RegisterPlayerInfo(FUniqueNetIdRepl PlayerID, FPlayerInfo&  PlayerInfo);

	UFUNCTION(BlueprintCallable)
	void UnRegisterPlayerInfo(FUniqueNetIdRepl PlayerID);

	UFUNCTION(BlueprintCallable)
	FPlayerInfo GetPlayerInfo(FUniqueNetIdRepl PlayerID);

	UFUNCTION(BlueprintCallable)
	void ChangePlayerInfo(FUniqueNetIdRepl PlayerID, FPlayerInfo& PlayerInfo);

	UFUNCTION(BlueprintCallable)
	void SetCurrentLevelTag(FGameplayTag LevelTag) { CurrentLevelTag = LevelTag; }

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetCurrentLevelTag() { return CurrentLevelTag; }
	
protected:
	TMap<FUniqueNetIdRepl, FPlayerInfo> PlayerInfos;
	FGameplayTag CurrentLevelTag;
};
