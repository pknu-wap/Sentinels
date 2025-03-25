// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "STGameplayTags.h"
#include "STGameState.generated.h"

class USTMissionBase;

USTRUCT()
struct FMissionInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag MissionTag;

	UPROPERTY()
	TObjectPtr<USTMissionBase> Mission;
};

UCLASS()
class SENTINELS_LS_API ASTGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASTGameState();

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
public:
	UFUNCTION(BlueprintCallable)
	void ActivateMission(FGameplayTag InMissionTag);
	UFUNCTION(BlueprintCallable)
	bool IsMissionCleared(FGameplayTag InMissionTag);

public:
	UFUNCTION(BlueprintCallable)
	void RegisterMission(FGameplayTag InMissionTag, TSubclassOf<USTMissionBase> MissionSubClass);

	UFUNCTION(BlueprintCallable)
	void UnRegisterMission(FGameplayTag InMissionTag);

	UFUNCTION()
	void OnMissionEnded(FGameplayTag InMissionTag, bool IsCleared);

	UFUNCTION(NetMulticast, Reliable)
	void OnMissionEnded_Multicast(FGameplayTag InMissionTag, bool IsCleared);
	
public:
	USTMissionBase* GetMission(FGameplayTag InMissionTag);

private:
	UPROPERTY(Replicated)
	TArray<FMissionInfo> Missions;

public:
	/*
		Update Mission Condition
	*/
	virtual void UpdateEliminatedMonsterInfo(int MonsterID);
	virtual void UpdateObjectDestroyedInfo(int ObjectID);
	virtual void UpdateAcquiredQuestItemInfo(int ItemID);
	virtual void UpdateRescueHostageInfo(int NPCID);
	virtual void UpdateRepairRiftInfo(int RiftID);
};
