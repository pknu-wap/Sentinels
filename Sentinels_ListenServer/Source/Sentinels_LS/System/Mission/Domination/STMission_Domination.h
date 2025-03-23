// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "System/Mission/STMissionBase.h"
#include "Tickable.h"
#include "STMission_Domination.generated.h"

USTRUCT(BlueprintType)
struct FEliminatedMonsterInfo
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
class SENTINELS_LS_API USTMission_Domination : public USTMissionBase
{
	GENERATED_BODY()

public:
	USTMission_Domination();

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
	virtual void UpdateEliminatedMonsterInfo_Server_Implementation(int MonsterID) override;

	UFUNCTION()
	void OnRep_MonsterInfos();

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	int MaxMissionTime = 30.f;

	float CurrentMissionTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<AActor>> SpawnerInfos;

	UPROPERTY(ReplicatedUsing = OnRep_MonsterInfos, EditAnywhere, BlueprintReadOnly)
	TArray<FEliminatedMonsterInfo> MonsterInfos;
};
