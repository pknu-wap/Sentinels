// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sentinels_LSGameMode.h"
#include "STEnums.h"
#include "STGameMode_MissionBase.generated.h"

class ASTMissionSection;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartVoting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVoteUpdated, EPortalType, portalType, int, voteCount);

UCLASS()
class SENTINELS_LS_API ASTGameMode_MissionBase : public ASentinels_LSGameMode
{
	GENERATED_BODY()
	
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual void BeginPlay() override;

/*
	Delegates	
*/
public:
	UPROPERTY(BlueprintAssignable)
	FOnStartVoting Delegate_StartVoting;

	UPROPERTY(BlueprintAssignable)
	FOnVoteUpdated Delegate_VoteUpdated;

/*
	Initial Misison
*/
protected:
	UFUNCTION(BlueprintCallable)
	void InitMissionInfos();

protected:
	UPROPERTY()
	TArray<ASTMissionSection*> CachedMissionSections;

	UPROPERTY()
	ASTMissionSection* CurrentMissionSection;


/*
	Select Mission or Boss
*/
public:
	UFUNCTION()
	void OnMissionEnded(class USTMissionBase* InMission, bool IsCleared);

	UFUNCTION()
	void StartVoting();

	UFUNCTION(BlueprintCallable)
	void VoteToMission(const APlayerController* PC);

	UFUNCTION(BlueprintCallable)
	void VoteToBoss(const APlayerController* PC);

	void DetermineMissionOrBoss();

	void TeleportPlayersToPlayerStarts(const TArray<APlayerStart*>& inPlayerStarts);

protected:
	UPROPERTY()
	FTimerHandle Handle_MissionOrBoss;

	UPROPERTY()
	TSet<int32> MissionVotedPlayers;

	UPROPERTY()
	TSet<int32> BossVotedPlayers;
};
