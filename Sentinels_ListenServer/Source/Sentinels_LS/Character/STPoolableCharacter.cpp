// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/STPoolableCharacter.h"
#include "Character/Enemy/STEnemyBase_AIController.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"

ASTPoolableCharacter::ASTPoolableCharacter(const FObjectInitializer& object_initializer)
	: Super(object_initializer)
{
}

void ASTPoolableCharacter::Activate(const FVector ActivateLocation, const FRotator ActivateRotation)
{
	bIsActivated = true;

	GetMesh()->SetEnableGravity(true);
	if (UCapsuleComponent* CapsuleComp = GetComponentByClass<UCapsuleComponent>())
	{
		CapsuleComp->SetEnableGravity(true);
	}

	ASTEnemyBase_AIController* AIController = Cast<ASTEnemyBase_AIController>(GetController());
	if (AIController)
	{
		AIController->StartAILogic(ActivateLocation, ActivateRotation);
	}

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

void ASTPoolableCharacter::Deactivate()
{
	bIsActivated = false;

	GetMesh()->SetEnableGravity(false);
	if (UCapsuleComponent* CapsuleComp = GetComponentByClass<UCapsuleComponent>())
	{
		CapsuleComp->SetEnableGravity(false);
	}

	ASTEnemyBase_AIController* AIController = Cast<ASTEnemyBase_AIController>(GetController());
	if (AIController)
	{
		UBrainComponent* brainComponnet = AIController->GetBrainComponent();
		if(brainComponnet) 
			brainComponnet->StopLogic(FString("Deactivated"));
	}

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}
