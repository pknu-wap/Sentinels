// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnEnemyComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENTINELS_LS_API USpawnEnemyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnEnemyComponent();


/*
    Spawn Enemy
*/
public:
    void StartSpawnEnemy();
    void StopSpawnEnemy();

protected:
    void SpawnEnemy();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    TArray<TSubclassOf<APawn>> SpawnPawnClasses;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    int SpawnRate = 5;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    float SpawnPeriod = 2.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    float SpawnableRadius_Outer = 1000.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    float SpawnableRadius_Inner = 700.f;


protected:
    FTimerHandle handle;
};
