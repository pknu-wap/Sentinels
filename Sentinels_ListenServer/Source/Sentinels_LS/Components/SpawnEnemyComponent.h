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
    void SetShouldLoop(bool inValue) { bShouldLoop = inValue; }

    UFUNCTION(BlueprintCallable)
    void StartSpawnEnemy();

    UFUNCTION(BlueprintCallable)
    void StopSpawnEnemy();

protected:
    void SpawnEnemy();

    UFUNCTION()
    void OnEnemyDied(AActor* DiedEnemy);

protected:
    /*
        Enemy Should Set Target as Owner When Spawned
    */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    bool bShouldTargetOwner = true;

    /*
        Class Array for Enemy
    */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    TArray<TSubclassOf<APawn>> SpawnPawnClasses;

    /*
        Spawn Pawns of SpawnRate per Spawn Period
    */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    bool bShouldLoop = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    int SpawnRate = 5;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    float SpawnPeriod = 2.f;

    /*
        Spawn Pawn between Outer Circle and Inner Circle
    */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    float SpawnableRadius_Outer = 1000.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    float SpawnableRadius_Inner = 700.f;

    /*
        Max Spawnable Num
    */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    int MaxSpawnCount = 32;

    UPROPERTY(VisibleAnywhere)
    int CurrentSpawned = 0;

    UPROPERTY()
    TArray<AActor*> SpawnedEnemys;

protected:
    FTimerHandle handle;
};
