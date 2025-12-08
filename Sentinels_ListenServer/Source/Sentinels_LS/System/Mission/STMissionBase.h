// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "System/NetworkObject.h"
#include "Tickable.h"
#include "GameplayTagContainer.h"
#include "STEnums.h"
#include "STMissionBase.generated.h"

class USTMissionConditionBase;
class USTMissionBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMissionEnded, USTMissionBase*, Mission, bool, IsSuccessed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMissionStateChanged, USTMissionBase*, Mission, EMissionProgressState, NewState);

UCLASS()
class SENTINELS_LS_API USTMissionBase : public UNetworkObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	USTMissionBase();

protected:
	virtual void BeginDestroy() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:
	// Spawn Mission Object
	UFUNCTION(BlueprintImplementableEvent)
	void OnMissionRegistered();

	UFUNCTION(BlueprintCallable)
	void RegisterMission();

	// Show Widget & Set Mission
	UFUNCTION(BlueprintImplementableEvent)
	void OnMissionActivated();

	UFUNCTION(BlueprintCallable)
	void ActivateMission();

	// Hide Widget & Clear Mission
	UFUNCTION(BlueprintImplementableEvent)
	void OnMissionDeactivated(bool IsCleared);

	UFUNCTION(BlueprintCallable)
	void DeactivateMission(bool IsCleared);

	// Check Mission is clearable
	UFUNCTION(BlueprintCallable)
	void CheckMissionClearable();

	// Define Mission Clear
	UFUNCTION(BlueprintCallable)
	bool IsMissionCleared();

protected:
	UFUNCTION(NetMulticast, Reliable)
	void MissionEnded_Multicast(bool IsCleared);

public:
	EMissionProgressState GetProgressState() const { return ProgressState; }

protected:
	UFUNCTION()
	void OnRep_ProgressState();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MissionName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MissionTag;

	UPROPERTY(ReplicatedUsing = OnRep_ProgressState, VisibleAnywhere, BlueprintReadOnly)
	EMissionProgressState ProgressState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<USTMissionConditionBase>> SubclassOfMissionConditions;

	UPROPERTY(Replicated, BlueprintReadOnly)
	TArray<USTMissionConditionBase*> MissionConditions;
	

	/*
		On Mission Ended Delegate
	*/
public:
	UPROPERTY(BlueprintAssignable)
	FOnMissionEnded Delegate_MissionEnded;
	
	UPROPERTY(BlueprintAssignable)
	FOnMissionStateChanged Delegate_MissionStateChanged;


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