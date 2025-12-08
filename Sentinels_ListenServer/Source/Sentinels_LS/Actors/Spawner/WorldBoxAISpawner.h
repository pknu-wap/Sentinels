// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SpawnEnemyComponent.h"
#include "Containers/Queue.h"
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

protected:
    UFUNCTION()
    void BoxBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    UFUNCTION()
    void BoxEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int NumOfOverlappedPlayers;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<AActor*> Players;

/*
       Spawn Enemy
*/
public:
    UFUNCTION(BlueprintCallable)
    void StartSpawnEnemy();

    UFUNCTION(BlueprintCallable)
    void StopSpawnEnemy();

protected:
    void TrySpawnAI();
    void SpawnEnemy(int InfoIdx);
    bool IsVectorInBoundingBox(const FVector& InLocation) const;
    bool IsInFrontalCone(const FVector& locationToCheck, const FVector& originLocation, const FVector& forwardVector, float angleDeg) const;
    bool GetSpawnNavLocation(int infoIdx, FNavLocation& OutLocation) const;
    bool GetSpawnNavLocationInBox(int infoIdx, FNavLocation& OutLocation) const;
    bool GetSpawnNavLocationForPlayer(int playerIdx, int infoIdx, FNavLocation& OutLocation) const;

protected:
    UFUNCTION()
    void OnEnemyDied(AActor* DiedEnemy);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<class UBoxComponent> Box;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    bool bCanActivated = true;

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
    TQueue<int> SpawnReserveQue;

    UPROPERTY()
    TSet<int> SpawnReserveSet;

    UPROPERTY()
    FTimerHandle SpawnHandle;

    float CurrentSpawned = 0;
};
