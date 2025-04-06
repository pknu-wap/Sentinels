// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Notify/ANS_EnableRootMotionRotation.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UANS_EnableRootMotionRotation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character)
	{
		UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
		if (MovementComp)
		{
			MovementComp->bAllowPhysicsRotationDuringAnimRootMotion = true;

			DefaultRotationRate = MovementComp->RotationRate;
			MovementComp->RotationRate = NewRotationRate;
		}
	}
}

void UANS_EnableRootMotionRotation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character)
	{
		UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
		if (MovementComp)
		{
			MovementComp->bAllowPhysicsRotationDuringAnimRootMotion = false;

			MovementComp->RotationRate = DefaultRotationRate;
		}
	}
}
