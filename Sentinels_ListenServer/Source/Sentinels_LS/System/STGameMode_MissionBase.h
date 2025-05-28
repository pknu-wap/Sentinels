// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sentinels_LSGameMode.h"
#include "STGameMode_MissionBase.generated.h"

class ASTMissionSection;

UCLASS()
class SENTINELS_LS_API ASTGameMode_MissionBase : public ASentinels_LSGameMode
{
	GENERATED_BODY()
	
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	ASTMissionSection* CurrentMissionSection;
};
