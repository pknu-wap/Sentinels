// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "STWorldSpawnSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API USTWorldSpawnSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	void PlayerNumUpdated(int NewPlayerNum);
	bool CanSpawnCharacter() const;
	void NewCharacterSpawned(AActor* NewSpawnedCharacter);
	void CharacterDeactivated(AActor* DeactivatedCharacter);

protected:
	UPROPERTY()
	int MaxSpawnableCharacters = 0;

	UPROPERTY()
	int CurrentSpawnedCharacters = 0;
};
