// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/MissionCondition/STMissionConditionBase.h"
#include "STKillCountCondition.generated.h"

USTRUCT(BlueprintType)
struct FKillCountInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MonsterID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Required = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Current = 0;
};

UCLASS()
class SENTINELS_LS_API USTKillCountCondition : public USTMissionConditionBase
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

	UFUNCTION()
	void OnRep_KillInfos();

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	int MaxMissionTime = 30.f;

	float CurrentMissionTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<AActor>> SpawnerInfos;

	UPROPERTY(ReplicatedUsing = OnRep_KillInfos, EditAnywhere, BlueprintReadOnly)
	TArray<FKillCountInfo> KillInfos;
};
