// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "STGameState_FinalBoss.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossCleared);

UCLASS()
class SENTINELS_LS_API ASTGameState_FinalBoss : public AGameStateBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(NetMulticast, Reliable)
	void OnBossCleared_Multicast();

protected:
	UPROPERTY(BlueprintAssignable)
	FOnBossCleared Delegate_OnBossCleared;
};
