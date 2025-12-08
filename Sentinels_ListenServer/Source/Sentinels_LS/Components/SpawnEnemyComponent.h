// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STStructs.h"
#include "SpawnEnemyComponent.generated.h"

struct FNavLocation;
class ASTEnemyBase;
class ASTPoolableCharacter;
class ACharacterObjectPool;

DECLARE_MULTICAST_DELEGATE(FOnEnemyAllDied);

USTRUCT(BlueprintType)
struct SENTINELS_LS_API FSpawnInfo 
{
    GENERATED_BODY()

    /*
       if Enemy set target as player, Only find player which is not so far
   */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    float MaxDistanceToPlayer = 3000.f;

    /*
        Class Array for Enemy
    */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    TArray<TSubclassOf<ASTPoolableCharacter>> SpawnPawnClasses;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    TArray<FDropInfo> AdditionalDropInfos;

    /*
        Spawn Pawns of SpawnRate per Spawn Period
    */
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
    int MaxSpawnCount = 12;

    UPROPERTY(VisibleAnywhere)
    int CurrentSpawned = 0;

    UPROPERTY()
    TSet<ASTPoolableCharacter*> SpawnedCharacters;

    UPROPERTY()
    TArray<ACharacterObjectPool*> ObjectPools;

    FTimerHandle TimerHandle;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENTINELS_LS_API USpawnEnemyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnEnemyComponent();

protected:
    virtual void BeginPlay() override;

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
    void SpawnEnemy(int InfoIdx);
    bool GetSpawnNavLocation(int InfoIdx, FNavLocation& OutLocation) const;
    void SetTarget(int InfoIdx, ASTEnemyBase* inEnemy);

    ACharacter* GetRandomPlayerCharacter(int InfoIdx, ASTEnemyBase* inEnemy) const;

    UFUNCTION()
    void OnEnemyDied(AActor* DiedEnemy);

public:
    FOnEnemyAllDied Delegate_OnEnemyAllDied;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    bool bShouldLoop = true;

    /*
        Enemy Should Set Target as Owner When Spawned
    */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    bool bShouldTargetOwner = true;

    /*
        Spawn Informations
    */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    TArray<FSpawnInfo> SpawnInfos;

private:
    float CurrentSpawned = 0;
};
