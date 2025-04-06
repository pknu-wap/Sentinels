// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/STPawnBase.h"

// Sets default values
ASTPawnBase::ASTPawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASTPawnBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASTPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASTPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

