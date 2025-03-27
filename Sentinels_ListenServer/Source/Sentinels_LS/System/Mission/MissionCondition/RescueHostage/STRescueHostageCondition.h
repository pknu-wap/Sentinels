// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/MissionCondition/STMissionConditionBase.h"
#include "STRescueHostageCondition.generated.h"

class AInteractableNPC;
class ASpawnPointBase;

USTRUCT(BlueprintType)
struct FHostageCInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NPCID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AInteractableNPC> SubClassOfNPC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsRescued = false;
};


UCLASS()
class SENTINELS_LS_API USTRescueHostageCondition : public USTMissionConditionBase
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
	void OnRep_HostageInfos();

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	int MaxMissionTime = 120.f;

	float CurrentMissionTime;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASpawnPointBase> SubClassOfSpawnPoint;

	UPROPERTY(ReplicatedUsing = OnRep_HostageInfos, EditAnywhere, BlueprintReadOnly)
	TArray<FHostageCInfo> HostageInfos;
};
