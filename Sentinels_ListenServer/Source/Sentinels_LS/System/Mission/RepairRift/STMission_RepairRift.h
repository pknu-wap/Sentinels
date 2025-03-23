// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/STMissionBase.h"
#include "STMission_RepairRift.generated.h"

class AInteractableRift;
class ASpawnPoint_Rift;

USTRUCT(BlueprintType)
struct FRepairRiftInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int RiftID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AInteractableRift> SubClassOfRift;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsRepaired = false;
};

UCLASS()
class SENTINELS_LS_API USTMission_RepairRift : public USTMissionBase
{
	GENERATED_BODY()
	
public:
	USTMission_RepairRift();

	/*
		Tickable Interface
	*/
protected:
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	/*
		Mission Interface
	*/
public:
	// Show Widget & Set Mission
	virtual void ActivateMission() override;

	// Hide Widget & Clear Mission
	virtual void DeactivateMission(bool IsCleared) override;

	// Define Mission Clear
	virtual bool IsMissionCleared() override;

public:
	virtual void UpdateRepairRiftInfo(int RiftID) override;

	UFUNCTION()
	void OnRep_RepairedRiftInfos();

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	int MaxMissionTime = 120.f;

	float CurrentMissionTime;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASpawnPoint_Rift> SubClassOfSpawnPoint;

	UPROPERTY(ReplicatedUsing = OnRep_RepairedRiftInfos, EditAnywhere, BlueprintReadOnly)
	TArray<FRepairRiftInfo> RepairedRiftInfos;
};
