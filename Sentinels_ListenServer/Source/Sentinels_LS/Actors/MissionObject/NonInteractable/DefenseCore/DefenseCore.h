// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MissionObject/MissionObject.h"
#include "DefenseCore.generated.h"

class UBoxComponent;
class USpawnEnemyComponent;

UCLASS()
class SENTINELS_LS_API ADefenseCore : public AMissionObject
{
	GENERATED_BODY()

public:
    ADefenseCore();

protected:
    virtual void BeginPlay() override;

public:
    void StopSpawnEnemy();

    /*
        Box Trigger
    */
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UBoxComponent> TriggerBoxComponent;

    UFUNCTION()
    void TriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    /*
        Spawn Enemy
    */
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<USpawnEnemyComponent> SpawnEnemyComp;
};
