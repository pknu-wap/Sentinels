// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "STStructs.h"
#include "EnhancementSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API UEnhancementSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	const FEnhancementInfo* GetEnhancementInfo(FGameplayTag InEnhancementtag) const;

protected:
	UPROPERTY()
	UDataTable* EnhancementDB_Total;

	UPROPERTY()
	TMap<FGameplayTag, FEnhancementInfo> EnhancementMap;
};
