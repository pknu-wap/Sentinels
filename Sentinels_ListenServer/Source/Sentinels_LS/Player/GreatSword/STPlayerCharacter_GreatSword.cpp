// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GreatSword/STPlayerCharacter_GreatSword.h"
#include "Player/STPlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkillComponent.h"
#include "Character/Enemy/STEnemyBase.h"
#include "STGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "MotionWarpingComponent.h"

void ASTPlayerCharacter_GreatSword::BeginPlay()
{
	Super::BeginPlay();

	USTPlayerAnimInstance* AnimInst = Cast<USTPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInst)
	{
		AnimInst->Delegate_SetWarpTarget_Passive.AddUObject(this, &ASTPlayerCharacter_GreatSword::SetWrapTarget_Passive);
	}
}

void ASTPlayerCharacter_GreatSword::AdjustFinalDamage(float& DamageAmount, FDamageEvent const& DamageEvent, AActor* DamagedActor)
{
	Super::AdjustFinalDamage(DamageAmount, DamageEvent, DamagedActor);

	ASTEnemyBase* Enemy = Cast<ASTEnemyBase>(DamagedActor);
	if (Enemy && Enemy->HasTag(FSTGameplayTags::Get().Character_State_Stunned))
	{
		DamageAmount = DamageAmount * 1.5f;
	}
}

void ASTPlayerCharacter_GreatSword::OnAttackSuccess_Server_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AActor* DamagedActor)
{
	Super::OnAttackSuccess_Server_Implementation(DamageAmount, DamageEvent, DamagedActor);
}

void ASTPlayerCharacter_GreatSword::Skill_Passive_Pressed()
{
	if (!CanDoSkill()) return;
	Skill_Passive_Pressed_Server();
}

void ASTPlayerCharacter_GreatSword::Skill_Passive_Pressed_Server_Implementation()
{
	Super::Skill_Passive_Pressed_Server_Implementation();

	// Capsule Trace Forward

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	FVector start = GetActorLocation();
	FVector end = start + TraceRange_Passive * GetActorForwardVector();

	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn);

	FHitResult hitResult;
	UKismetSystemLibrary::SphereTraceSingle(this, start, end, TraceRadius_Passive,
		TraceType, false, IgnoreActors,
		EDrawDebugTrace::None, hitResult, true);

	if (hitResult.bBlockingHit)
	{
		ASTCharacterBase* newTarget = Cast<ASTCharacterBase>(hitResult.GetActor());
		if (newTarget && newTarget->HasTag(FSTGameplayTags::Get().Character_State_Stunned))
		{
			TargetActor_Passive = newTarget;

			AddTag(FSTGameplayTags::Get().Character_State_Skill);
			Skill_Passive_Pressed_Multicast();
			PlayMontage_Skill_Passive();
		}
	}
}

void ASTPlayerCharacter_GreatSword::SetWrapTarget_Passive()
{
	if (HasAuthority())
	{
		UMotionWarpingComponent* motionWarpComp = GetComponentByClass<UMotionWarpingComponent>();
		if (motionWarpComp && IsValid(TargetActor_Passive))
		{
			FVector towardVec = (TargetActor_Passive->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();

			FMotionWarpingTarget WarpTarget;
			WarpTarget.Name = FName("WarpTarget");
			WarpTarget.Location = TargetActor_Passive->GetActorLocation();
			WarpTarget.Rotation = GetControlRotation();
			WarpTarget.Rotation = towardVec.ToOrientationRotator();

			motionWarpComp->AddOrUpdateWarpTarget(WarpTarget);
		}
	}
}
