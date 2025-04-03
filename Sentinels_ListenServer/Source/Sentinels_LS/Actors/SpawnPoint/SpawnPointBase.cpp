// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SpawnPoint/SpawnPointBase.h"

// Sets default values
ASpawnPointBase::ASpawnPointBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

bool ASpawnPointBase::HasTag(FGameplayTag InTag) const
{
	return TagContainer.HasTag(InTag);
}

