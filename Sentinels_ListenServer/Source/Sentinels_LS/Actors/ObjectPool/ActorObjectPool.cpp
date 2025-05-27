// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ObjectPool/ActorObjectPool.h"
#include "Actors/STPoolableActor.h"

// Sets default values
AActorObjectPool::AActorObjectPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActorObjectPool::BeginPlay()
{
	Super::BeginPlay();
	
}

void AActorObjectPool::InitPool(TSubclassOf<ASTPoolableActor> InObjectClass, int PoolSize)
{
	UWorld* world = GetWorld();
	if (!world) return;

	ObjectPool.Reserve(PoolSize);
	ObjectClass = InObjectClass;

	for (int i = 0; i < PoolSize; i++)
	{
		ASTPoolableActor* actor = Cast<ASTPoolableActor>(world->SpawnActor<ASTPoolableActor>(ObjectClass));
		if (actor)
		{
			actor->Deactivate();
			ObjectPool.Add(actor);
		}
	}
}

