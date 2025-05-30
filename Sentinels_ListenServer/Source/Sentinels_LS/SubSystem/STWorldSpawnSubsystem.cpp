// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/STWorldSpawnSubsystem.h"

void USTWorldSpawnSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);


}

void USTWorldSpawnSubsystem::PlayerNumUpdated(int NewPlayerNum)
{
	MaxSpawnableCharacters = FMath::Clamp(NewPlayerNum * 7, 20, 40);
}

bool USTWorldSpawnSubsystem::CanSpawnCharacter() const
{
	return CurrentSpawnedCharacters < MaxSpawnableCharacters;
}

void USTWorldSpawnSubsystem::NewCharacterSpawned(AActor* NewSpawnedCharacter)
{
	CurrentSpawnedCharacters = FMath::Clamp(CurrentSpawnedCharacters + 1, 0, MaxSpawnableCharacters);
}

void USTWorldSpawnSubsystem::CharacterDeactivated(AActor* DeactivatedCharacter)
{
	CurrentSpawnedCharacters = FMath::Clamp(CurrentSpawnedCharacters - 1, 0, MaxSpawnableCharacters);
}
