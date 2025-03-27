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
    virtual void Tick(float fDeltaTime) override;

    /*
        Box Trigger
    */
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<USphereComponent> TriggerSphereComponent;

    UFUNCTION()
    void TriggerSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void TriggerSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    /*
        Domination
    */
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float RequireTimeForDomination = 10.f;

private:
    int OverlappedPlayer = 0;
    float currentDominationTime = 0.f;



    /*
        Spawn Enemy
    */
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<USpawnEnemyComponent> SpawnEnemyComp;

};
