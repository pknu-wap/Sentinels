// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MissionObject/Interactable/InteractableMissionObject.h"
#include "EscortObject.generated.h"

class ASplineRouteActor;
class USpawnEnemyComponent;
class UInteractComponent;

UCLASS()
class SENTINELS_LS_API AEscortObject : public AInteractableMissionObject
{
	GENERATED_BODY()
       
public:
    AEscortObject();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    /*
        Interactive Interface
    */
protected:
    virtual void Interact_Implementation(UInteractComponent* InteractingComponent) override;

protected:
    UPROPERTY(EditAnywhere)
    int EscortObjectIdx = 0;


    /*
        Box Component
    */
protected:
    UFUNCTION()
    void BoxBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void BoxEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<class UBoxComponent> BoxComp;


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
