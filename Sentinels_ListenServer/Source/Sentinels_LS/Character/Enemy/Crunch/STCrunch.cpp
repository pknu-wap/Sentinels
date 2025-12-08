// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Crunch/STCrunch.h"
#include "Character/Enemy/Crunch/STCrunchAnimInstance.h"
#include "Character/Enemy/STEnemyBase_AIController.h"
#include "Actors/Projectile/ProjectileBase.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionWarpingComponent.h"

void ASTCrunch::StartSpawnMissiles()
{
	if (HasAuthority())
	{
		PlayAnimMontage(Montage_SpawnMissile);
		GetWorldTimerManager().SetTimer(Handle_SpawnMissiles, this, &ASTCrunch::SpawnMissile, 0.1f, true);
	}
}

void ASTCrunch::SetDashWarpTarget(const FName WarpTargetName, const FTransform InTransform)
{
	// Set Destination Location using MotionWarp
	if (UMotionWarpingComponent* motionWarpComp = GetComponentByClass<UMotionWarpingComponent>())
	{
		motionWarpComp->AddOrUpdateWarpTargetFromTransform(WarpTargetName, InTransform);
	}
}

void ASTCrunch::StartDash()
{
	// Stop Movement
	GetCharacterMovement()->StopMovementImmediately();

	// Set EMovementMode::Move_Walking using AnimNotify 
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Update WalkSpeed
	GetCharacterMovement()->MaxWalkSpeed = 900.f;

	// Update Animation
	if (USTCrunchAnimInstance* AnimInst = Cast<USTCrunchAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInst->StartDash();
	}
}

void ASTCrunch::EndDash()
{
	// Stop Movement
	GetCharacterMovement()->StopMovementImmediately();

	// Set EMovementMode::Move_Walking using AnimNotify 
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Update WalkSpeed
	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	// Update Animation
	if (USTCrunchAnimInstance* AnimInst = Cast<USTCrunchAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInst->EndDash();
	}
}

void ASTCrunch::SpawnMissile()
{
	FNavLocation OutNavLocation;

	UNavigationSystemV1 * NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	if (!NavSystem) return;

	if (NavSystem->GetRandomReachablePointInRadius(GetOwner()->GetActorLocation(), 10000.f, OutNavLocation))
	{
		AProjectileBase* Missile = GetWorld()->SpawnActor<AProjectileBase>(SubclassOfMissile, 
			OutNavLocation.Location + FVector(0, 0, 10000), FRotator::ZeroRotator);
		if (Missile)
		{
			Missile->SetTargetLocation(OutNavLocation.Location);
			SpawnedMissiles++;
		}
	}

	if (SpawnedMissiles >= MaxMissiles)
	{
		SpawnedMissiles = 0;
		GetWorldTimerManager().ClearTimer(Handle_SpawnMissiles);
	}
}
