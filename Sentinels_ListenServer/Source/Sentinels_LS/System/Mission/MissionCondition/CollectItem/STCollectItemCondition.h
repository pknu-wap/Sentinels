// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/MissionCondition/STMissionConditionBase.h"
#include "STCollectItemCondition.generated.h"

class ASpawnPointBase;
class ASpawner;

USTRUCT(BlueprintType)
struct FCollectedItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int ItemID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Required = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Current = 0;
};

USTRUCT(BlueprintType)
struct FSpawnerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASpawner> SpawnerClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int SpawnPointIdx = 0;
};

UCLASS()
class SENTINELS_LS_API USTCollectItemCondition : public USTMissionConditionBase
{
	GENERATED_BODY()

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSatisfied_Implementation() override;

public:
	virtual void MissionActivated_Implementation() override;
	virtual void MissionDeactivated_Implementation(bool IsCleared) override;

	UFUNCTION()
	void OnRep_CollectedItemInfos();

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	int MaxMissionTime = 30.f;

	float CurrentMissionTime;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASpawnPointBase> SubClassOfSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSpawnerInfo> SpawnerInfos;

	UPROPERTY(ReplicatedUsing = OnRep_CollectedItemInfos, EditAnywhere, BlueprintReadOnly)
	TArray<FCollectedItemInfo> CollectedItemInfos;
};
