// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GreatSword/STPlayerCharacter_GreatSword.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkillComponent.h"
#include "Character/Enemy/STEnemyBase.h"
#include "STGameplayTags.h"

void ASTPlayerCharacter_GreatSword::AdjustFinalDamage(float& DamageAmount, FDamageEvent const& DamageEvent, AActor* DamagedActor)
{
	Super::AdjustFinalDamage(DamageAmount, DamageEvent, DamagedActor);

	ASTEnemyBase* Enemy = Cast<ASTEnemyBase>(DamagedActor);
	if (Enemy && Enemy->HasTag(FSTGameplayTags::Get().Character_State_Stunned))
	{
		DamageAmount = DamageAmount * 1.5f;
	}
}

void ASTPlayerCharacter_GreatSword::OnAttackSuccess_Server_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::OnAttackSuccess_Server_Implementation(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
