// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/STMissionBase.h"
#include "STMission_RescueHostage.generated.h"

class AInteractableNPC;
class ASpawnPoint_NPC;

USTRUCT(BlueprintType)
struct FRescuedNPCInfo
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
class SENTINELS_LS_API USTMission_RescueHostage : public USTMissionBase
{
	GENERATED_BODY()
	
public:
	USTMission_RescueHostage();

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
	virtual void UpdateRescueHostageInfo_Server_Implementation(int NPCID) override;

	UFUNCTION()
	void OnRep_RescuedNPCInfos();

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	int MaxMissionTime = 120.f;

	float CurrentMissionTime;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASpawnPoint_NPC> SubClassOfSpawnPoint;

	UPROPERTY(ReplicatedUsing = OnRep_RescuedNPCInfos, EditAnywhere, BlueprintReadOnly)
	TArray<FRescuedNPCInfo> RescuedNPCInfos;
};
