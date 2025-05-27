// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "STGameplayTags.h"
#include "STStructs.h"
#include "STGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnServerTravelReady, FGameplayTag, LevelTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRepActivatedMission);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRepSubMissions);

class USTMissionBase;

USTRUCT(BlueprintType)
struct FMissionInfo
{
	GENERATED_BODY()

	FMissionInfo() {};

	FMissionInfo(class USTMissionBase* InMission, FGameplayTag InMissionTag)
		: Mission(InMission), MissionTag(InMissionTag)
	{};

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class USTMissionBase> Mission;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag MissionTag;
};

UCLASS()
class SENTINELS_LS_API ASTGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASTGameState();

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
	/*
		Main Mission
	*/
public:
	int GetClearedMissionNum() const;

public:
	UFUNCTION(BlueprintCallable)
	void ActivateRandomMission();

	UFUNCTION(BlueprintCallable)
	void ActivateMission(FGameplayTag InMissionTag);

	UFUNCTION(BlueprintCallable)
	bool IsMissionCleared(FGameplayTag InMissionTag);

public:
	UFUNCTION(BlueprintCallable)
	void RegisterRandomMissions(int RegisterMissionNum, const TArray<FRegisterMissionInfo>& AvailableMissionInfos);

	UFUNCTION(BlueprintCallable)
	void RegisterMission(FGameplayTag InMissionTag, TSubclassOf<USTMissionBase> MissionSubClass);

	UFUNCTION(BlueprintCallable)
	void UnRegisterMission(FGameplayTag InMissionTag, bool IsCleared);

	UFUNCTION()
	void OnMissionEnded(FGameplayTag InMissionTag, bool IsCleared);

	UFUNCTION(NetMulticast, Reliable)
	void OnMissionEnded_Multicast(FGameplayTag InMissionTag, bool IsCleared);

public:
	UFUNCTION(BlueprintCallable)
	USTMissionBase* GetMission(FGameplayTag InMissionTag);

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetCurrentLevelTag() { return CurrentLevelTag; }

	UFUNCTION(BlueprintCallable)
	void SetCurrentLevelTag(FGameplayTag NewLevelTag);

protected:
	UFUNCTION()
	void OnRep_ActivatedMission();

	UPROPERTY(BlueprintAssignable)
	FOnRepActivatedMission Delegate_OnRepActivatedMission;

private:
	UPROPERTY(Replicated)
	TArray<FMissionInfo> Missions;

	UPROPERTY(ReplicatedUsing = OnRep_ActivatedMission, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FMissionInfo> ActivatedMissions;

	/*
		Sub Mission
	*/
public:
	UFUNCTION(BlueprintCallable)
	void RegisterSubMissions(const TArray<FRegisterMissionInfo>& InSubMissionInfos);

	UFUNCTION(BlueprintCallable)
	void RegisterSubMission(FGameplayTag InMissionTag, TSubclassOf<USTMissionBase> SubMissionSubClass);

	UFUNCTION()
	void OnSubMissionEnded(FGameplayTag InMissionTag, bool IsCleared);

	UFUNCTION(NetMulticast, Reliable)
	void OnSubMissionEnded_Multicast(FGameplayTag InMissionTag, bool IsCleared);

protected:
	UFUNCTION()
	void OnRep_SubMissions();

	UPROPERTY(BlueprintAssignable)
	FOnRepSubMissions Delegate_OnRepSubMissions;

private:
	UPROPERTY(ReplicatedUsing = OnRep_SubMissions, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FMissionInfo> SubMissions;


public:
	UFUNCTION(BlueprintCallable)
	void TryServerTravel();

public:
	UPROPERTY(Replicated)
	FGameplayTag CurrentLevelTag;

	UPROPERTY(BlueprintAssignable)
	FOnServerTravelReady OnServerTravelReady;
};
