// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Notify/ANS_ConvertMovementMode.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UANS_ConvertMovementMode::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character)
	{
		UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
		if (MovementComp)
		{
			MovementComp->SetMovementMode(InMovementMode.GetValue());
		}
	}
}

void UANS_ConvertMovementMode::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character)
	{
		UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
		if (MovementComp)
		{
			MovementComp->SetMovementMode(OutMovementMode.GetValue());
		}
	}
}
