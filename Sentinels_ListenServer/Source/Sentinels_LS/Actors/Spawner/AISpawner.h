// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AISpawner.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API AAISpawner : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AAISpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	virtual void SpawnActors();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	TArray<TSubclassOf<APawn>> SpawnPawnClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	int SpawnRate = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	float SpawnPeriod = 2.f;

protected:
	FTimerHandle handle;
};
