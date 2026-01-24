// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sentinels_LSGameMode.h"
#include "STGameMode_FinalBoss.generated.h"

UCLASS()
class SENTINELS_LS_API ASTGameMode_FinalBoss : public ASentinels_LSGameMode
{
	GENERATED_BODY()
	
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

protected:
	UFUNCTION()
	void BossClearCallback(AActor* ClearedBoss);

	UFUNCTION()
	void ServerTravelToLobby();
};
