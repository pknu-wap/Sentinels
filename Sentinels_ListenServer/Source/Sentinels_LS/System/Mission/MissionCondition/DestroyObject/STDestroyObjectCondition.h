// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/MissionCondition/STMissionConditionBase.h"
#include "STDestroyObjectCondition.generated.h"

class ADestructibleObject;

USTRUCT(BlueprintType)
struct FDestructibleObjectInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int ObjectID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ADestructibleObject> SubClassOfDestructibleObject;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDestroyed = false;
};

UCLASS()
class SENTINELS_LS_API USTDestroyObjectCondition : public USTMissionConditionBase
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
	TArray<FDestructibleObjectInfo> DestructibleObjectInfos;

	/*
	*	Time Limit
	*/
protected:
	void TimeLimitEnded();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeLimit = 120.f;

	FTimerHandle handle;
};
