// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MissionObject/MissionObject.h"
#include "DominationPoint.generated.h"

class USphereComponent;
class USpawnEnemyComponent;

UCLASS()
class SENTINELS_LS_API ADominationPoint : public AMissionObject
{
	GENERATED_BODY()
	
public:
    ADominationPoint();

protected:
    virtual void BeginPlay() override;

    /*
        Box Trigger
    */
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<USphereComponent> TriggerSphereComponent;

    UFUNCTION()
    void TriggerSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    /*
        Spawn Enemy
    */
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<USpawnEnemyComponent> SpawnEnemyComp;

};
