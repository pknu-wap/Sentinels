// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/MissionCondition/STMissionConditionBase.h"
#include "STCollectItemCondition.generated.h"

class ASpawnPoint_Spawner;
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
	virtual bool IsSatisfied() override;

public:
	virtual void MissionActivated() override;
	virtual void MissionDeactivated(bool IsCleared) override;

	virtual void UpdateAcquiredQuestItemInfo(int InItemID) override;

	UFUNCTION()
	void OnRep_CollectedItemInfos();

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	int MaxMissionTime = 30.f;

	float CurrentMissionTime;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASpawnPoint_Spawner> SubClassOfSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSpawnerInfo> SpawnerInfos;

	UPROPERTY(ReplicatedUsing = OnRep_CollectedItemInfos, EditAnywhere, BlueprintReadOnly)
	TArray<FCollectedItemInfo> CollectedItemInfos;
};
