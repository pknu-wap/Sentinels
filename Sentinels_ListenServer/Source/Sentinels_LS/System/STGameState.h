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

	FGameplayTag MissionTag;
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

public:
	void UpdateEliminatedMonsterInfo(int MonsterID);
	void UpdateObjectDestroyedInfo(int ObjectID);
	void UpdateAcquiredQuestItemInfo(int ItemID);
	void UpdateRescueHostageInfo(int NPCID);
	void UpdateRepairRiftInfo(int RiftID);
	
protected:
	USTMissionBase* GetMission(FGameplayTag InMissionTag);

private:
	UPROPERTY(Replicated)
	TArray<FMissionInfo> Missions;
};
