// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GreatSword/STPlayerCharacter_GreatSword.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkillComponent.h"

void ASTPlayerCharacter_GreatSword::Skill_Q_Pressed()
{
	USkillComponent* SkillComp = GetController()->GetComponentByClass<USkillComponent>();
	if (SkillComp && SkillComp->CanActivateSkill(1))
	{
		SkillComp->ActivateSkill(1);

		PlayMontage_Skill_Q();
		Skill_Q_Pressed_Server();
	}
}

void ASTPlayerCharacter_GreatSword::Skill_Q_Pressed_Server_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("ASTPlayerCharacter_GreatSword : Skill_Q_Pressed_Server Called!"));

	Skill_Q_Pressed_Multicast();
}

void ASTPlayerCharacter_GreatSword::Skill_Q_Pressed_Multicast_Implementation()
{
	if (!IsLocallyControlled())
	{
		UE_LOG(LogTemp, Display, TEXT("ASTPlayerCharacter_GreatSword : Skill_Q_Pressed_Multicast Called!"));

		UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
		if (AnimInst)
		{
			AnimInst->Montage_Play(Montage_Skill_Q);
		}
	}
}

void ASTPlayerCharacter_GreatSword::Skill_W_Pressed()
{
}

void ASTPlayerCharacter_GreatSword::Skill_W_Pressed_Server_Implementation()
{
}

void ASTPlayerCharacter_GreatSword::Skill_W_Pressed_Multicast_Implementation()
{
}

void ASTPlayerCharacter_GreatSword::Skill_E_Pressed()
{
}

void ASTPlayerCharacter_GreatSword::Skill_E_Pressed_Server_Implementation()
{
}

void ASTPlayerCharacter_GreatSword::Skill_E_Pressed_Multicast_Implementation()
{
}

void ASTPlayerCharacter_GreatSword::Skill_R_Pressed()
{
}

void ASTPlayerCharacter_GreatSword::Skill_R_Pressed_Server_Implementation()
{
}

void ASTPlayerCharacter_GreatSword::Skill_R_Pressed_Multicast_Implementation()
{
}
