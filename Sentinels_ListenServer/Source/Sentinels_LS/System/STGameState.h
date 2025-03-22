// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "STGameplayTags.h"
#include "STGameState.generated.h"

class ISTMissionBase;

USTRUCT()
struct FMissionInfo
{
	GENERATED_BODY()

	FGameplayTag MissionTag;
	TObjectPtr<ISTMissionBase> Mission;
};

UCLASS()
class SENTINELS_LS_API ASTGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
public:
	void ActivateMission(FGameplayTag InMissionTag);
	void DeactivateMission(FGameplayTag InMissionTag);
	bool IsMissionCleared(FGameplayTag InMissionTag);

protected:
	ISTMissionBase* GetMission(FGameplayTag InMissionTag);

private:
	UPROPERTY(Replicated)
	TArray<FMissionInfo> Missions;

};
