// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Katana/STPlayerCharacter_Katana.h"
#include "Player/STPlayerAnimInstance.h"
#include "Actors/Projectile/ProjectileBase.h"
#include "MotionWarpingComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "STGameplayTags.h"
#include "SubSystem/STProjectilePoolingSubSystem.h"
#include "Components/STPlayerStatusComponent.h"
#include "Character/Enemy/STEnemyBase.h"

void ASTPlayerCharacter_Katana::BeginPlay()
{
	Super::BeginPlay();

	USTPlayerAnimInstance* AnimInst = Cast<USTPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInst)
	{
		AnimInst->Delegate_SetWarpTarget_Q.AddUObject(this, &ASTPlayerCharacter_Katana::SetWrapTarget_Q);
		AnimInst->Delegate_SpawnSlash_Q.AddUObject(this, &ASTPlayerCharacter_Katana::SpawnSlash_Q);
		AnimInst->Delegate_SetWarpTarget_Passive.AddUObject(this, &ASTPlayerCharacter_Katana::SetWrapTarget_Passive);
		AnimInst->Delegate_ApplyDamage.AddUObject(this, &ASTPlayerCharacter_Katana::ApplyPassiveDamage);
	}

	if (HasAuthority())
	{
		USTProjectilePoolingSubSystem* PoolingSystem = GetWorld()->GetSubsystem<USTProjectilePoolingSubSystem>();
		PoolingSystem->InitProjectilePool(this, SubclassOfSlash_R, 50);
	}
}

void ASTPlayerCharacter_Katana::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME_CONDITION(ASTPlayerCharacter_Katana, MaxMovementSpeed, COND_OwnerOnly);
}

void ASTPlayerCharacter_Katana::AdjustFinalDamage(float& DamageAmount, FDamageEvent const& DamageEvent, AActor* DamagedActor)
{
	Super::AdjustFinalDamage(DamageAmount, DamageEvent, DamagedActor);
}

void ASTPlayerCharacter_Katana::OnAttackSuccess_Server_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AActor* DamagedActor) 
{
	Super::OnAttackSuccess_Server_Implementation(DamageAmount, DamageEvent, DamagedActor);
}

void ASTPlayerCharacter_Katana::Skill_Passive_Pressed()
{
	if (!CanDoSkill()) return;
	Skill_Passive_Pressed_Server();
}

void ASTPlayerCharacter_Katana::Skill_Passive_Pressed_Server_Implementation()
{
	Super::Skill_Passive_Pressed_Server_Implementation();

	AllTargetActors_Passive.Empty();

	// Capsule Trace Forward
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	FVector start = GetActorLocation();
	FVector end = start + TraceRange_Passive * GetActorForwardVector();

	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn);

	TArray<FHitResult> hitResults;
	UKismetSystemLibrary::SphereTraceMulti(this, start, end, TraceRadius_Passive,
		TraceType, false, IgnoreActors,
		EDrawDebugTrace::None, hitResults, true);

	TargetActor_Passive = nullptr;
	for (auto& hitResult : hitResults)
	{
		ASTCharacterBase* newTarget = Cast<ASTCharacterBase>(hitResult.GetActor());
		if (newTarget && newTarget->HasTag(FSTGameplayTags::Get().Character_State_Bleed))
		{
			AllTargetActors_Passive.Add(newTarget);

			if (TargetActor_Passive)
			{
				double originDist = FVector::DistSquared2D(TargetActor_Passive->GetActorLocation(), GetActorLocation());
				double newDist = FVector::DistSquared2D(newTarget->GetActorLocation(), GetActorLocation());
				if (originDist < newDist)
				{
					TargetActor_Passive = newTarget;
				}
			}
			else
			{
				TargetActor_Passive = newTarget;
			}
		}
	}

	if (TargetActor_Passive)
	{
		AddTag(FSTGameplayTags::Get().Character_State_Skill);
		Skill_Passive_Pressed_Multicast();
	}
}

void ASTPlayerCharacter_Katana::Skill_Passive_Pressed_Multicast_Implementation()
{
	PlayMontage_Skill_Passive();
}

void ASTPlayerCharacter_Katana::ApplyPassiveDamage()
{
	if (HasAuthority() && StatusComponent)
	{
		for (AActor* actor : AllTargetActors_Passive)
		{
			if (ASTCharacterBase* character = Cast<ASTCharacterBase>(actor))
			{
				// Applay Damage (ATK * 0.5 * NumOfTag)
				int numOfTag = character->GetNumOfTag(FSTGameplayTags::Get().Character_State_Bleed);

				if (numOfTag > 0)
				{
					UGameplayStatics::ApplyDamage(actor, StatusComponent->ATK * 0.5 * numOfTag, GetController(), this, UDamageType::StaticClass());

					// Clear Bleed Tag
					character->ClearTag_Server(FSTGameplayTags::Get().Character_State_Bleed);
					character->UpdateStateWidget_Server(FSTGameplayTags::Get().Character_State_Bleed, false);
				}
			}
		}
	}
}

void ASTPlayerCharacter_Katana::SetWrapTarget_Passive()
{
	if (HasAuthority())
	{
		UMotionWarpingComponent* motionWarpComp = GetComponentByClass<UMotionWarpingComponent>();
		if (motionWarpComp && TargetActor_Passive)
		{
			FVector towardVec = (TargetActor_Passive->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();

			FMotionWarpingTarget WarpTarget;
			WarpTarget.Name = FName("WarpTarget");
			WarpTarget.Location = TargetActor_Passive->GetActorLocation() + towardVec * WarpDistance_Passive;
			WarpTarget.Rotation = GetControlRotation();

			/*
				Find Wall
			*/
			TArray<AActor*> IgnoreActors;
			IgnoreActors.Add(TargetActor_Passive);

			FVector start = TargetActor_Passive->GetActorLocation();
			FVector end = start + TraceRange_Passive * towardVec;

			ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel4);

			FHitResult hitResult;
			UKismetSystemLibrary::SphereTraceSingle(this, start, end, 25,
				TraceType, false, IgnoreActors,
				EDrawDebugTrace::ForDuration, hitResult, true);

			if (hitResult.bBlockingHit)
			{
				WarpTarget.Location = hitResult.ImpactPoint;
			}

			/*
				Set Warp Target
			*/
			DrawDebugCapsule(GetWorld(), WarpTarget.Location, 50, 88, FRotator::ZeroRotator.Quaternion(), FColor::Blue, false, 5.f);
			motionWarpComp->AddOrUpdateWarpTarget(WarpTarget);

		}
	}
}

void ASTPlayerCharacter_Katana::SetWrapTarget_Q()
{
	if (HasAuthority())
	{
		UMotionWarpingComponent* motionWarpComp = GetComponentByClass<UMotionWarpingComponent>();
		if (motionWarpComp)
		{
			FMotionWarpingTarget WarpTarget;
			WarpTarget.Name = FName("WarpTarget");
			WarpTarget.Location = GetActorLocation() + GetControlRotation().Vector() * WarpDistance_Q;
			WarpTarget.Rotation = GetControlRotation();

			SpawnLocation_Slash_Q = GetActorLocation() + GetControlRotation().Vector() * WarpDistance_Q * 0.5f;

			motionWarpComp->AddOrUpdateWarpTarget(WarpTarget);
		}
	}
}

void ASTPlayerCharacter_Katana::SpawnSlash_Q()
{
	AActor* Slash = GetWorld()->SpawnActor<AActor>(SubclassOfSlash_Q, SpawnLocation_Slash_Q, FRotator::ZeroRotator);
	if (Slash)
	{
		Slash->SetInstigator(this);
	}
}
