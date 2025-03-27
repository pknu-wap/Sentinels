// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/NetworkObject.h"
#include "GameplayTagContainer.h"
#include "STMissionConditionBase.generated.h"

/**
 * 
 */
UCLASS(ABSTRACT)
class SENTINELS_LS_API USTMissionConditionBase : public UNetworkObject
{
	GENERATED_BODY()
	
public:
	virtual void MissionActivated() {};
	virtual void MissionDeactivated(bool IsCleared) {};

	virtual void UpdateDominationInfo(int ObjectID, bool Success) {};
	virtual void UpdateEliminatedMonsterInfo(int MonsterID) {};
	virtual void UpdateObjectDestroyedInfo(int ObjectID) {};
	virtual void UpdateAcquiredQuestItemInfo(int ItemID) {};
	virtual void UpdateRescueHostageInfo(int NPCID) {};
	virtual void UpdateRepairRiftInfo(int RiftID) {};
	virtual void UpdateEscortInfo(int ObjectID, bool IsSuccessed) { };

	virtual bool IsSatisfied() { return false; };

public:
	void SetMissionTag(FGameplayTag InTag) { MissionTag = InTag; };

protected:
	FGameplayTag MissionTag;
};
