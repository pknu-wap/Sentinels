// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "System/NetworkObject.h"
#include "Tickable.h"
#include "GameplayTagContainer.h"
#include "STMissionBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMissionEnded, FGameplayTag, MissionTag, bool, IsSuccessed);

UCLASS()
class SENTINELS_LS_API USTMissionBase : public UNetworkObject, public FTickableGameObject
{
	GENERATED_BODY()

protected:
	virtual void BeginDestroy() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:
	// Show Widget & Set Mission
	virtual void ActivateMission();

	// Hide Widget & Clear Mission
	virtual void DeactivateMission(bool IsCleared);

	// Define Mission Clear
	virtual bool IsMissionCleared() { return false; };

	/*
		Update Mission Info
	*/
public:
	// Domination || Eliminate EliteMonster || Final Defense
	virtual void UpdateEliminatedMonsterInfo(int MonsterID) {};

	// Destroy Object 
	virtual void UpdateObjectDestroyedInfo(int ObjectID) {};

	// Collect QuestItems
	virtual void UpdateAcquiredQuestItemInfo(int ItemID) {};

	// Resque Hostage
	virtual void UpdateRescueHostageInfo(int NPCID) {};

	// Repair Rift
	virtual void UpdateRepairRiftInfo(int RiftID) {};
	

	/*
		On Mission Ended Delegate
	*/
public:
	FOnMissionEnded Delegate_MissionEnded;


protected:
	UFUNCTION()
	virtual void OnRep_bIsMisionActivated();

	UPROPERTY(ReplicatedUsing = OnRep_bIsMisionActivated, EditAnywhere, BlueprintReadWrite)
	bool bIsMisionActivated = false;



/*
	Tickable Interface
*/
protected:
	virtual TStatId GetStatId() const override { return UObject::GetStatID(); }
	virtual void Tick(float DeltaTime) override {}
	virtual bool IsTickable() const override { return bIsTickable; }
	virtual bool IsTickableInEditor() const override { return true; }
	virtual bool IsTickableWhenPaused() const override { return bTickableWhenPaused; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsTickable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bTickableWhenPaused;
};