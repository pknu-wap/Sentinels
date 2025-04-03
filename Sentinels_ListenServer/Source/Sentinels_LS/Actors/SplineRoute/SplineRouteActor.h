// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineRouteActor.generated.h"

class USplineComponent;

UCLASS()
class SENTINELS_LS_API ASplineRouteActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineRouteActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	float GetSplineLength();
	FVector GetSplineLocationAtDistance(float length);
	FRotator GetSplineRotationAtDistance(float length);

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USplineComponent> Spline;
};
