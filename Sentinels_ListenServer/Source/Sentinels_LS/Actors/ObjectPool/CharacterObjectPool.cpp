// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ObjectPool/CharacterObjectPool.h"
#include "Character/STPoolableCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
ACharacterObjectPool::ACharacterObjectPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACharacterObjectPool::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterObjectPool::InitPool(TSubclassOf<ASTPoolableCharacter> InObjectClass, int PoolSize)
{
	UWorld* world = GetWorld();
	if (!world) return;

	ObjectPool.Reserve(PoolSize);
	ObjectClass = InObjectClass;
	
	for(int i = 0; i < PoolSize; i++)
	{
		ASTPoolableCharacter* character = Cast<ASTPoolableCharacter>(UAIBlueprintHelperLibrary::SpawnAIFromClass(this, ObjectClass, nullptr,
			FVector::ZeroVector, FRotator::ZeroRotator, true));
		if (character)
		{
			character->Deactivate();
			ObjectPool.Add(character);
		}
	}
}