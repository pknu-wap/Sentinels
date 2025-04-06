// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/NetworkObject.h"
#include "GameplayTagContainer.h"
#include "STMissionConditionBase.generated.h"

class ASpawnPointBase;

UCLASS(ABSTRACT)
class SENTINELS_LS_API USTMissionConditionBase : public UNetworkObject
{
	GENERATED_BODY()
	
public:
	virtual void MissionRegistered() {};
	virtual void MissionActivated() {};
	virtual void MissionDeactivated(bool IsCleared) {};
	virtual bool IsSatisfied() { return false; };

public:
	void SetMissionTag(FGameplayTag InTag) { MissionTag = InTag; };

	void GetAllSpawnPointsWithTag(FGameplayTag InTag, TArray<ASpawnPointBase*>& OutActors) const;

protected:
	FGameplayTag MissionTag;
};
