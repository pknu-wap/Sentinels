// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SplineRoute/SplineRouteActor.h"
#include "Components/SplineComponent.h"

// Sets default values
ASplineRouteActor::ASplineRouteActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Spline = CreateDefaultSubobject<USplineComponent>(FName("SplineComp"));
	Spline->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASplineRouteActor::BeginPlay()
{
	Super::BeginPlay();
	
}

float ASplineRouteActor::GetSplineLength()
{
	if (!Spline)
		return 0.0f;
	return Spline->GetSplineLength();
}

FVector ASplineRouteActor::GetSplineLocationAtDistance(float length)
{
	if (!Spline) return FVector();

	return Spline->GetLocationAtDistanceAlongSpline(length, ESplineCoordinateSpace::World);
}

FRotator ASplineRouteActor::GetSplineRotationAtDistance(float length)
{
	if (!Spline) return FRotator::ZeroRotator;

	FVector Direction = Spline->GetDirectionAtDistanceAlongSpline(length, ESplineCoordinateSpace::World);
	FRotator NewRotation = FRotationMatrix::MakeFromX(Direction).Rotator();

	return NewRotation;
}
