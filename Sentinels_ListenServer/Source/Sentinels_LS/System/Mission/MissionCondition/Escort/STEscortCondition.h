// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/MissionCondition/STMissionConditionBase.h"
#include "STEscortCondition.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API USTEscortCondition : public USTMissionConditionBase
{
	GENERATED_BODY()
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const override;
	virtual bool IsSatisfied_Implementation() override;

public:
	virtual void MissionActivated_Implementation() override;
	virtual void MissionDeactivated_Implementation(bool IsCleared) override;

	UFUNCTION()
	void ConditionUpdated(int ObjectID, bool Success);


public:
	bool Successed = false;

};
