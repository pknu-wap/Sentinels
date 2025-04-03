// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Spawner/Spawner.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(handle, this, &ASpawner::SpawnActors, SpawnPeriod, true);
	}
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawner::SpawnActors()
{
	int rand;
	for (int i = 0; i < SpawnRate; i++)
	{
		rand = UKismetMathLibrary::RandomIntegerInRange(0, SpawnActorClasses.Num() - 1);
		if (SpawnActorClasses[rand])
		{
			GetWorld()->SpawnActor<AActor>(SpawnActorClasses[rand], GetActorLocation(), GetActorRotation());
		}
	}
}

