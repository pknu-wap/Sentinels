// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/STCharacterBase.h"

// Sets default values
ASTCharacterBase::ASTCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASTCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASTCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASTCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

