// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/MissionCondition/STMissionConditionBase.h"
#include "Tickable.h"
#include "STDominationCondition.generated.h"

class AInteractableDominationCore;
class ASpawnPoint_DominationCore;

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

	virtual void UpdateDominationInfo(int ObjectID, bool Success) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASpawnPoint_DominationCore> SubclassOfSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AInteractableDominationCore> SubclassOfDominatinoCore;

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
