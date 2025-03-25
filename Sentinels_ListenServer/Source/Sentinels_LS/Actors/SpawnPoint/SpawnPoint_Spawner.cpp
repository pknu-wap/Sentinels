// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SpawnPoint/SpawnPoint_Spawner.h"

// Sets default values
ASpawnPoint_Spawner::ASpawnPoint_Spawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnPoint_Spawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnPoint_Spawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

