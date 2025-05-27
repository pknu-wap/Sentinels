// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Notify/ANS_DisableCollision.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

void UANS_DisableCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character)
	{
		CapsuleCollision = Character->GetCapsuleComponent()->GetCollisionEnabled();
		MeshCollision = Character->GetMesh()->GetCollisionEnabled();

		Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		// Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::);
		// Character->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UANS_DisableCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		// Character->GetMesh()->SetCollisionEnabled(MeshCollision);
	}
}
