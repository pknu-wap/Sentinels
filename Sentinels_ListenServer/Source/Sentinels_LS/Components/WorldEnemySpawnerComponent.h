// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnEnemyComponent.h"
#include "WorldEnemySpawnerComponent.generated.h"

struct FNavLocation;
class ASTEnemyBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENTINELS_LS_API UWorldEnemySpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWorldEnemySpawnerComponent();

protected:
    virtual void BeginPlay() override;

/*
    Spawn Enemy
*/
public:
    UFUNCTION(BlueprintCallable)
    void StartSpawnEnemy();

    UFUNCTION(BlueprintCallable)
    void StopSpawnEnemy();

protected:
    void SpawnEnemy(int InfoIdx);
    bool GetSpawnNavLocation(int infoIdx, FNavLocation& OutLocation) const;

protected:
    UFUNCTION()
    void OnEnemyDied(AActor* DiedEnemy);

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
