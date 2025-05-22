// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/STPoolableCharacter.h"
#include "Character/Enemy/STEnemyBase_AIController.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASTPoolableCharacter::ASTPoolableCharacter(const FObjectInitializer& object_initializer)
	: Super(object_initializer)
{
}

void ASTPoolableCharacter::Activate(const FVector ActivateLocation, const FRotator ActivateRotation)
{
	bIsActivated = true;

	SetActorLocation(ActivateLocation); SetActorRotation(ActivateRotation);

	// Enable Tick
	GetMesh()->SetComponentTickEnabled(true);
	GetCharacterMovement()->SetComponentTickEnabled(true);
	SetActorTickEnabled(true);

	GetMesh()->SetEnableGravity(true);
	if (UCapsuleComponent* CapsuleComp = GetComponentByClass<UCapsuleComponent>())
	{
		CapsuleComp->SetEnableGravity(true);
	}

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void ASTPoolableCharacter::Deactivate()
{
	bIsActivated = false;

	// Disable Tick
	// GetMesh()->SetAnimationMode(EAnimationMode::AnimationCustomMode);
	GetMesh()->SetComponentTickEnabled(false);
	GetCharacterMovement()->SetComponentTickEnabled(false);
	SetActorTickEnabled(false);

	// Disable Gravity
	GetMesh()->SetEnableGravity(false);
	if (UCapsuleComponent* CapsuleComp = GetComponentByClass<UCapsuleComponent>())
	{
		CapsuleComp->SetEnableGravity(false);
	}

	// Disable Behavior Tree
	ASTEnemyBase_AIController* AIController = Cast<ASTEnemyBase_AIController>(GetController());
	if (AIController)
	{
		UBrainComponent* brainComponnet = AIController->GetBrainComponent();
		if(brainComponnet) 
			brainComponnet->StopLogic(FString("Deactivated"));
	}

	// Disable Collision
	SetActorEnableCollision(false);

	// Disable Visibility
	SetActorHiddenInGame(true);
}
