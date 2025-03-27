// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/MissionCondition/STMissionConditionBase.h"
#include "Tickable.h"
#include "STDominationCondition.generated.h"

class AInteractableDefenseCore;
class ASpawnPoint_DefenseCore;

UCLASS()
class SENTINELS_LS_API USTDominationCondition : public USTMissionConditionBase
{
	GENERATED_BODY()
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const override;
	virtual bool IsSatisfied() override;

public:
	virtual void MissionActivated() override;
	virtual void MissionDeactivated(bool IsCleared) override;


};
