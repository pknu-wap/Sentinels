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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRegisterMission);

class USTMissionBase;
class ASTMissionSection;

USTRUCT(BlueprintType)
struct FMissionInfo
{
	GENERATED_BODY()

	FMissionInfo() {};

	FMissionInfo(class USTMissionBase* InMission, FGameplayTag InMissionTag, ASTMissionSection* InMissionSection)
		: Mission(InMission), MissionTag(InMissionTag), MissionSection(InMissionSection)
	{};

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class USTMissionBase> Mission;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag MissionTag;

	UPROPERTY()
	ASTMissionSection* MissionSection;
};

USTRUCT(BlueprintType)
struct FSubMissionInfo
{
	GENERATED_BODY()

	FSubMissionInfo() {};

	FSubMissionInfo(class USTMissionBase* InMission, FGameplayTag InMissionTag)
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
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
	/*
		Main Mission
	*/
public:
	UFUNCTION(BlueprintCallable)
	int GetClearedMissionNum() const;

	ASTMissionSection* GetActivatedMissionSection() const;

public:
	UFUNCTION(BlueprintCallable)
	void ActivateRandomMission();

	UFUNCTION(BlueprintCallable)
	void ActivateSubMission(FGameplayTag InMissionTag);

	UFUNCTION(BlueprintCallable)
	void ActivateMission(ASTMissionSection* InMissionSection);

public:
	UFUNCTION(BlueprintCallable)
	void RegisterRandomMissions(int RegisterMissionNum, const TArray<FRegisterMissionInfo>& AvailableMissionInfos, ASTMissionSection* MissionSection);

	UFUNCTION(BlueprintCallable)
	void RegisterMission(FGameplayTag InMissionTag, TSubclassOf<USTMissionBase> MissionSubClass, ASTMissionSection* MissionSection);

	UPROPERTY(BlueprintAssignable)
	FOnRegisterMission OnRegisterMission;
	
	//UFUNCTION(BlueprintCallable)
	//void UnRegisterMission(FGameplayTag InMissionTag, bool IsCleared);

	UFUNCTION()
	void OnMissionEnded(USTMissionBase* InMission, bool IsCleared);

	UFUNCTION(NetMulticast, Reliable)
	void OnMissionEnded_Multicast(USTMissionBase* InMission, bool IsCleared);

public:
	UFUNCTION(BlueprintCallable)
	USTMissionBase* GetMission(FGameplayTag InMissionTag);

	UFUNCTION(BlueprintCallable)
	USTMissionBase* GetSubMission(FGameplayTag InMissionTag);

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
	UPROPERTY(Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
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
	void OnSubMissionEnded(USTMissionBase* InMission, bool IsCleared);

	UFUNCTION(NetMulticast, Reliable)
	void OnSubMissionEnded_Multicast(USTMissionBase* InMission, bool IsCleared);

protected:
	UFUNCTION()
	void OnRep_SubMissions();

	UPROPERTY(BlueprintAssignable)
	FOnRepSubMissions Delegate_OnRepSubMissions;

private:
	UPROPERTY(ReplicatedUsing = OnRep_SubMissions, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FSubMissionInfo> SubMissions;


public:
	UFUNCTION(BlueprintCallable)
	void TryServerTravel();

public:
	UPROPERTY(BlueprintAssignable)
	FOnServerTravelReady OnServerTravelReady;

protected:
	UPROPERTY(Replicated)
	FGameplayTag CurrentLevelTag;


};
