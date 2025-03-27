// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interact/InteractableActor.h"
#include "InteractableDominationCore.generated.h"

class UBoxComponent;
class USpawnEnemyComponent;

UCLASS()
class SENTINELS_LS_API AInteractableDominationCore : public AInteractableActor
{
	GENERATED_BODY()
	
public:
    AInteractableDominationCore();

protected:
    virtual void BeginPlay() override;

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
