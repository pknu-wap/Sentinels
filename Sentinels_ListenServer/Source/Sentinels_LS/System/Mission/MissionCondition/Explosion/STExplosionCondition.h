// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/MissionCondition/STMissionConditionBase.h"
#include "STExplosionCondition.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API USTExplosionCondition : public USTMissionConditionBase
{
	GENERATED_BODY()
	
public:
	virtual bool IsSatisfied_Implementation() override;

public:
	virtual void MissionActivated_Implementation() override;

protected:
	UFUNCTION()
	void OnEventOccur(FGameplayTag InTag);

protected:
	bool bIsExplosionOccured = false;
};
