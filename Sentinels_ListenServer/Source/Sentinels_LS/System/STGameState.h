// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "STGameplayTags.h"
#include "STGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAllPlayerIsReady, FGameplayTag, LevelTag);

class USTMissionBase;

USTRUCT(BlueprintType)
struct FRegisterMissionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag MissionTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USTMissionBase> MissionSubClass;
};

USTRUCT()
struct FMissionInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag MissionTag;

	UPROPERTY()
	TObjectPtr<USTMissionBase> Mission;
};

UCLASS()
class SENTINELS_LS_API ASTGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASTGameState();

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
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
	void UnRegisterMission(FGameplayTag InMissionTag);

	UFUNCTION()
	void OnMissionEnded(FGameplayTag InMissionTag, bool IsCleared);

	UFUNCTION(NetMulticast, Reliable)
	void OnMissionEnded_Multicast(FGameplayTag InMissionTag, bool IsCleared);

public:
	USTMissionBase* GetMission(FGameplayTag InMissionTag);

	FGameplayTag GetCurrentLevelTag() { return CurrentLevelTag; }
	void SetCurrentLevelTag(FGameplayTag NewLevelTag);

private:
	UPROPERTY(Replicated)
	TArray<FMissionInfo> Missions;

	UPROPERTY(Replicated)
	FMissionInfo ActivatedMission;

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

private:
	UPROPERTY(Replicated)
	TArray<FMissionInfo> SubMissions;


public:
	UFUNCTION(BlueprintCallable)
	bool CanServerTravel();

	UFUNCTION(BlueprintCallable)
	void TryServerTravel();

public:
	UPROPERTY(Replicated)
	FGameplayTag CurrentLevelTag;

	UPROPERTY(BlueprintAssignable)
	FOnAllPlayerIsReady OnAllPlayerIsReady;
};
