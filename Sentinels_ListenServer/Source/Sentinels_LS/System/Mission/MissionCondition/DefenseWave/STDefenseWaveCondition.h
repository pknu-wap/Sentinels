// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/MissionCondition/STMissionConditionBase.h"
#include "STDefenseWaveCondition.generated.h"

class ADefenseCore;
class ASpawnPointBase;

UCLASS()
class SENTINELS_LS_API USTDefenseWaveCondition : public USTMissionConditionBase
{
	GENERATED_BODY()

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const override;
	virtual bool IsSatisfied() override;

public:
	virtual void MissionActivated() override;
	virtual void MissionDeactivated(bool IsCleared) override;

	UFUNCTION()
	void ConditionUpdated(int ObjectID, bool Success);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASpawnPointBase> SubclassOfSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ADefenseCore> SubclassOfDefenseCore;

private:
	bool bCoreIsDestroyed = false;

	/*
	*	Time Limit
	*/
protected:
	void TimeLimitEnded();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DominationTimeLimit = 120.f;

	FTimerHandle handle;
};
