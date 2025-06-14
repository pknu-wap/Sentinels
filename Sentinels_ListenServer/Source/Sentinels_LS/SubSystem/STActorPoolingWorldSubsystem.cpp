// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/STActorPoolingWorldSubsystem.h"


USTActorPoolingWorldSubsystem::USTActorPoolingWorldSubsystem()
{
}

void USTActorPoolingWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USTActorPoolingWorldSubsystem::InitActorPool(UObject* WorldContextObject, TSubclassOf<ASTPoolableActor> InClass, int InPoolSize)
{
	AActorObjectPool** ObjectPoolPPtr = ActorPools.Find(InClass);
	if (ObjectPoolPPtr || !WorldContextObject) return;

	UWorld* world = WorldContextObject->GetWorld();
	if (world)
	{
		AActorObjectPool* ObjectPool = world->SpawnActor<AActorObjectPool>(AActorObjectPool::StaticClass());
		if (ObjectPool)
		{
			ObjectPool->InitPool(InClass, InPoolSize);
			ActorPools.Add(InClass, ObjectPool);
		}
	}
}

AActor* USTActorPoolingWorldSubsystem::K2_GetActor(TSubclassOf<ASTPoolableActor> InClass, FVector Location, FRotator Rotation)
{
	return GetActor<ASTPoolableActor>(InClass, Location, Rotation);
}
