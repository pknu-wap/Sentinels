// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/STPoolableActor.h"

// Sets default values
ASTPoolableActor::ASTPoolableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ASTPoolableActor::Activate(const FVector ActivateLocation, const FRotator ActivateRotation)
{
	bIsActivated = true;
	SetActorLocation(ActivateLocation);
	SetActorRotation(ActivateRotation);
}

void ASTPoolableActor::Deactivate()
{
	bIsActivated = false;
}
