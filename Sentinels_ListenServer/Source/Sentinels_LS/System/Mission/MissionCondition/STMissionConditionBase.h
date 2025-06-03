// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/NetworkObject.h"
#include "GameplayTagContainer.h"
#include "STMissionConditionBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConditionUpdated);

class ASpawnPointBase;
class USTMissionBase;

UCLASS(ABSTRACT)
class SENTINELS_LS_API USTMissionConditionBase : public UNetworkObject
{
	GENERATED_BODY()
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

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
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<AActor*> GetMissionObjects();

public:
	void InitializeCondition(USTMissionBase* InMission, FGameplayTag InTag) 
	{ Mission = InMission; MissionTag = InTag; };

	void GetAllSpawnPointsWithTag(FGameplayTag InTag, TArray<ASpawnPointBase*>& OutActors) const;

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	USTMissionBase* Mission;

	FGameplayTag MissionTag;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnConditionUpdated Delegate_ConditionUpdated;



/*
	Blueprint Helper Function		
*/
protected:
	UWorld* GetWorld() const override
	{
		if (IsTemplate() || !GetOuter())
		{
			return nullptr;
		}
		return GetOuter()->GetWorld();
	}
};
