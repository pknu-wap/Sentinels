// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/MissionCondition/STMissionConditionBase.h"
#include "Tickable.h"
#include "STDominationCondition.generated.h"

class AInteractableDefenseCore;
class ADominationPoint;

USTRUCT(BlueprintType)
struct FDominationPointInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int DominationPID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ADominationPoint> SubClassOfDominationPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDominated = false;

	UPROPERTY()
	ADominationPoint* DominationPoint = nullptr;
};

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

	UFUNCTION()
	void ConditionUpdated(int ObjectID, bool Success);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASpawnPointBase> SubclassOfSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDominationPointInfo> DominationPointInfos;

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
