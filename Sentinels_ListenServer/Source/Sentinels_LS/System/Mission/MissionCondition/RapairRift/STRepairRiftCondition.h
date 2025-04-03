// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/MissionCondition/STMissionConditionBase.h"
#include "STRepairRiftCondition.generated.h"

class ASpawnPointBase;
class ARift;

USTRUCT(BlueprintType)
struct FRiftInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int RiftID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ARift> SubClassOfRift;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsRepaired = false;
};

UCLASS()
class SENTINELS_LS_API USTRepairRiftCondition : public USTMissionConditionBase
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

	UFUNCTION()
	void OnRep_RiftInfos();

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	int MaxMissionTime = 120.f;

	float CurrentMissionTime;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASpawnPointBase> SubClassOfSpawnPoint;

	UPROPERTY(ReplicatedUsing = OnRep_RiftInfos, EditAnywhere, BlueprintReadOnly)
	TArray<FRiftInfo> RiftInfos;
};
