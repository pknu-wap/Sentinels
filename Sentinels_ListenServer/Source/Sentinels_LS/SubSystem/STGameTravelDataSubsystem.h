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
struct FPlayerSKMeshesRowName
{
	GENERATED_BODY()

	/*
		Player Customize Parts
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name_Head = "0";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name_Hood = "0";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name_LongHair = "0";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name_Glasses = "0";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name_UpperBody = "0";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name_Backpack = "0";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name_Hand_L = "0";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name_Hand_R = "0";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name_BottomBody = "0";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name_Feet = "0";
};

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESTClassType PlayerClass = ESTClassType::GreatSword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerSKMeshesRowName PlayerSKMeshesRowName;
};

UCLASS()
class SENTINELS_LS_API USTGameTravelDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	USTGameTravelDataSubsystem();

public:
	UFUNCTION(BlueprintCallable)
	void RegisterPlayerInfo(FUniqueNetIdRepl PlayerID, FPlayerInfo  PlayerInfo);

	UFUNCTION(BlueprintCallable)
	void UnRegisterPlayerInfo(FUniqueNetIdRepl PlayerID);

	UFUNCTION(BlueprintCallable)
	void ClearPlayerInfo() { PlayerInfos.Empty(); }

	UFUNCTION(BlueprintCallable)
	const FPlayerInfo& LoadPlayerInfo(FUniqueNetIdRepl PlayerID);

	UFUNCTION(BlueprintCallable)
	void SavePlayerCurrentClass(FUniqueNetIdRepl PlayerID, ESTClassType ClassType);

	UFUNCTION(BlueprintCallable)
	const UDataTable* GetSKMeshDT(ESKParts SKPart);

	UFUNCTION(BlueprintCallable)
	void SavePlayerSKMeshes(FUniqueNetIdRepl PlayerID, FPlayerSKMeshesRowName& SKMeshesRowName);

	UFUNCTION(BlueprintCallable)
	void SetLevelTag(FGameplayTag CurrentLevelTag) { LevelTag = CurrentLevelTag; }

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetLevelTag() { return LevelTag; }

	UFUNCTION(BlueprintCallable)
	void SetBossLevelName(FString LevelName) { BossLevelName = LevelName; }

	UFUNCTION(BlueprintCallable)
	FString GetBossLevelName() { return BossLevelName; }

protected:
	UPROPERTY()
	TMap<FUniqueNetIdRepl, FPlayerInfo> PlayerInfos;

	UPROPERTY()
	FGameplayTag LevelTag;

	UPROPERTY()
	FString BossLevelName;

	UDataTable* DT_Head;
	UDataTable* DT_Hood;
	UDataTable* DT_LongHair;
	UDataTable* DT_Glasses;
	UDataTable* DT_UpperBody;
	UDataTable* DT_Backpack;
	UDataTable* DT_Hand_L;
	UDataTable* DT_Hand_R;
	UDataTable* DT_BottomBody;
	UDataTable* DT_Feet;
};
