// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interact/InteractableActor.h"
#include "InteractableEscortObject.generated.h"

class USplineComponent;
class ASplineRouteActor;
class USpawnEnemyComponent;

UCLASS()
class SENTINELS_LS_API AInteractableEscortObject : public AInteractableActor
{
	GENERATED_BODY()

public:
    AInteractableEscortObject();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    /*
        Interactive Interface
    */
protected:
    virtual void Interact() override;

protected:
    UPROPERTY(EditAnywhere)
    int EscortObjectIdx = 0;

    /*
        Move Along Spine
    */

public:
    UFUNCTION(BlueprintCallable)
    void StartMoveAlongSpline();

    UFUNCTION(BlueprintCallable)
    void StopMove();

protected:
    UPROPERTY(EditAnywhere)
    TObjectPtr<ASplineRouteActor> SplineActor;

    UPROPERTY(EditAnywhere)
    float MoveSpeed;

private:
    bool bShouldMove = false;
    float TimePercent = 0.f;
    float DistanceAlongSpline = 0.f;


    /*
        Spawn Enemy
    */

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<USpawnEnemyComponent> SpawnEnemyComp;

};
