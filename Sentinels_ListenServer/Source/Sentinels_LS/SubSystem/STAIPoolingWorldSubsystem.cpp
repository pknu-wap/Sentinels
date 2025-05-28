// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/STAIPoolingWorldSubsystem.h"

void USTAIPoolingWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USTAIPoolingWorldSubsystem::InitCharacterPool(TSubclassOf<ASTPoolableCharacter> InClass, int InPoolSize)
{
	ACharacterObjectPool** CharacterPoolPPtr = CharacterPools.Find(InClass);
	if (CharacterPoolPPtr) return;

	UWorld* world = GetWorld();
	if (world)
	{
		ACharacterObjectPool* CharacterPool = world->SpawnActor<ACharacterObjectPool>(ACharacterObjectPool::StaticClass());
		if (CharacterPool)
		{
			CharacterPool->InitPool(InClass, InPoolSize);
			CharacterPools.Add(InClass, CharacterPool);
		}
	}
}
