// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Actors/ObjectPool/CharacterObjectPool.h"
#include "STAIPoolingWorldSubsystem.generated.h"

class ASTPoolableCharacter;
class ACharacterObjectPool;

UCLASS()
class SENTINELS_LS_API USTAIPoolingWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	void InitCharacterPool(TSubclassOf<ASTPoolableCharacter> InClass, int InPoolSize);

	template<typename T>
	T* GetCharacter(TSubclassOf<ASTPoolableCharacter> InClass, FVector Location = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator)
	{
		ACharacterObjectPool** ObjectPool = CharacterPools.Find(InClass);
		if (ObjectPool)
		{
			return (*ObjectPool)->GetCharacter<T>(Location, Rotation);
		}

		return nullptr;
	}

protected:
	UPROPERTY()
	TMap<TSubclassOf<ASTPoolableCharacter>, ACharacterObjectPool*> CharacterPools;
};
