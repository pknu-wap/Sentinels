// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Spawner/AISpawner.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"

AAISpawner::AAISpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AAISpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(handle, this, &AAISpawner::SpawnActors, SpawnPeriod, true);
	}
}

void AAISpawner::SpawnActors()
{
	int rand;
	for (int i = 0; i < SpawnRate; i++)
	{
		rand = UKismetMathLibrary::RandomIntegerInRange(0, SpawnPawnClasses.Num() - 1);
		if (SpawnPawnClasses[rand])
		{
			UAIBlueprintHelperLibrary::SpawnAIFromClass(this, SpawnPawnClasses[rand], nullptr, 
				GetActorLocation(), GetActorRotation());
		}
	}
}
