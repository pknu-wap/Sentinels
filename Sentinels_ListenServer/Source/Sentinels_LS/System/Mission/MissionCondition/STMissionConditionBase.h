// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/NetworkObject.h"
#include "GameplayTagContainer.h"
#include "STMissionConditionBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConditionUpdated);

class ASpawnPointBase;

UCLASS(ABSTRACT)
class SENTINELS_LS_API USTMissionConditionBase : public UNetworkObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void MissionRegistered();
	UFUNCTION(BlueprintNativeEvent)
	void MissionActivated();
	UFUNCTION(BlueprintNativeEvent)
	void MissionDeactivated(bool IsCleared);
	UFUNCTION(BlueprintNativeEvent)
	bool IsSatisfied();

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetConditionDescription();

public:
	void SetMissionTag(FGameplayTag InTag) { MissionTag = InTag; };

	void GetAllSpawnPointsWithTag(FGameplayTag InTag, TArray<ASpawnPointBase*>& OutActors) const;

protected:
	UPROPERTY(BlueprintAssignable)
	FOnConditionUpdated Delegate_ConditionUpdated;

	FGameplayTag MissionTag;
};
