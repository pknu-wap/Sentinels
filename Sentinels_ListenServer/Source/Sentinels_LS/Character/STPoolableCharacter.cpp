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

	if (!ActivateLocation.Equals(FVector::ZeroVector))
		SetActorLocation(ActivateLocation);
	
	if (!ActivateRotation.Equals(FRotator::ZeroRotator))
		SetActorRotation(ActivateRotation);

	// Enable Tick
	GetMesh()->Activate();
	GetCharacterMovement()->Activate();
	SetActorTickEnabled(true);

	AAIController* controller = Cast<AAIController>(GetController());
	if (controller)
	{
		controller->SetActorTickEnabled(true);
	}

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
	GetMesh()->Deactivate();
	GetCharacterMovement()->Deactivate();
	SetActorTickEnabled(false);

	// Disable Gravity
	GetMesh()->SetEnableGravity(false);
	if (UCapsuleComponent* CapsuleComp = GetComponentByClass<UCapsuleComponent>())
	{
		CapsuleComp->SetEnableGravity(false);
	}

	// Disable Tick & Behavior Tree
	AAIController* controller = Cast<AAIController>(GetController());
	if (controller)
	{
		controller->SetActorTickEnabled(false);

		UBrainComponent* brainComponnet = controller->GetBrainComponent();
		if(brainComponnet) 
			brainComponnet->StopLogic(FString("Deactivated"));
	}

	// Disable Collision
	SetActorEnableCollision(false);

	// Disable Visibility
	SetActorHiddenInGame(true);
}
