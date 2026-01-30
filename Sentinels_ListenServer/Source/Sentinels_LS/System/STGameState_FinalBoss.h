// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "STGameplayTags.h"
#include "STGameState_FinalBoss.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossCleared);

UCLASS()
class SENTINELS_LS_API ASTGameState_FinalBoss : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASTGameState_FinalBoss();
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(NetMulticast, Reliable)
	void OnBossCleared_Multicast();

	/*
		Level
	*/

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetLevelTag() { return LevelTag; }

protected:
	UPROPERTY(BlueprintAssignable)
	FOnBossCleared Delegate_OnBossCleared;

	UPROPERTY(Replicated)
	FGameplayTag LevelTag;
};
