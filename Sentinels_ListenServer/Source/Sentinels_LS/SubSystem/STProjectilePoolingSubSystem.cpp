// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/STProjectilePoolingSubSystem.h"
#include "Actors/Projectile/ProjectileBase.h"

USTProjectilePoolingSubSystem::USTProjectilePoolingSubSystem()
{
}

void USTProjectilePoolingSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);


}

void USTProjectilePoolingSubSystem::InitProjectilePool(UObject* WorldContextObject, TSubclassOf<AProjectileBase> InClass, int InPoolSize)
{
	AActorObjectPool** ObjectPoolPPtr = ProjectilePools.Find(InClass);
	if (ObjectPoolPPtr || !WorldContextObject) return;

	UWorld* world = WorldContextObject->GetWorld();
	if (world)
	{
		AActorObjectPool* ObjectPool = world->SpawnActor<AActorObjectPool>(AActorObjectPool::StaticClass());
		if (ObjectPool)
		{
			ObjectPool->InitPool(InClass, InPoolSize);
			ProjectilePools.Add(InClass, ObjectPool);
		}
	}
}
