// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SpawnPoint/SpawnPoint_Rift.h"

// Sets default values
ASpawnPoint_Rift::ASpawnPoint_Rift()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnPoint_Rift::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnPoint_Rift::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

