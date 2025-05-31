// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/GRIM/STGRIM.h"
#include "Character/STCharacterAnimInstanceBase.h"
#include "Character/Enemy/STEnemyBase_AIController.h"
#include "Actors/Projectile/ProjectileBase.h"

void ASTGRIM::BeginPlay()
{
	Super::BeginPlay();

	USTCharacterAnimInstanceBase* AnimInst = Cast<USTCharacterAnimInstanceBase>(GetMesh()->GetAnimInstance());
	if (AnimInst)
	{
		AnimInst->Delegate_UltimateFire.AddUObject(this, &ASTGRIM::UltimateFire);
	}
}

void ASTGRIM::ActivateUltimate_Server_Implementation()
{
	ActivateUltimate_Multicast();
	PlayUltimateMontage();
}

void ASTGRIM::ActivateUltimate_Multicast_Implementation()
{
	PlayUltimateMontage();
}

void ASTGRIM::PlayUltimateMontage()
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->Montage_Play(Montage_Ultimate);
	}
}

void ASTGRIM::UltimateFire()
{
	if (HasAuthority())
	{
		FVector SpawnLocation = GetMesh()->GetSocketLocation(FName("Muzzle_04"));

		ASTEnemyBase_AIController* controller = Cast<ASTEnemyBase_AIController>(GetController());
		AActor* TargetPlayer = controller->GetCurrentTarget();
		if (!TargetPlayer) return;

		FVector TargetLocation = TargetPlayer->GetActorLocation() - FVector(0, 0, 45.f);

		// Spawn Target Actor
		AActor* TargetActor = GetWorld()->SpawnActor<AActor>(SubclassOfTargetActor, TargetLocation, FRotator::ZeroRotator);
		if (TargetActor)
		{
			TargetActor->SetActorLocation(TargetLocation);
			// TargetActor->AttachToActor(TargetPlayer, FAttachmentTransformRules::KeepRelativeTransform);

			// Spawn Projectile && Set Homing Target
			AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(SubclassOfUltimateProjectile, SpawnLocation, FRotator::ZeroRotator);
			if (Projectile)
			{
				Projectile->SetInstigator(this);

				FVector FireDirection = GetActorForwardVector();
				FRotator FireRotation = FireDirection.Rotation();
				FireRotation.Pitch += 67.5f;

				Projectile->FireInDirection(FireRotation.Vector());
				Projectile->SetHomingTarget(TargetActor);
			}
		}
	}
}
