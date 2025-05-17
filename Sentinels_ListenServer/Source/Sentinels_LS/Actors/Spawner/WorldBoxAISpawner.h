// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SpawnEnemyComponent.h"
#include "WorldBoxAISpawner.generated.h"

UCLASS()
class SENTINELS_LS_API AWorldBoxAISpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldBoxAISpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
    bool IsVectorInBoundingBox(const FVector& InLocation) const;
    bool GetSpawnNavLocation(int infoIdx, FNavLocation& OutLocation) const;
    bool GetSpawnNavLocationInBox(int infoIdx, FNavLocation& OutLocation) const;

protected:
    UFUNCTION()
    void OnEnemyDied(AActor* DiedEnemy);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<class UBoxComponent> Box;

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
