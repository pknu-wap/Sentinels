// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SpawnPoint/SpawnPoint_DominationCore.h"

// Sets default values
ASpawnPoint_DominationCore::ASpawnPoint_DominationCore()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnPoint_DominationCore::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnPoint_DominationCore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

