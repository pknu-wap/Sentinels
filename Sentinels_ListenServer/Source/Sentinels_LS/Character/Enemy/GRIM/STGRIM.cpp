// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/GRIM/STGRIM.h"
#include "Character/Enemy/GRIM/STGRIMAnimInstance.h"
#include "Character/Enemy/STEnemyBase_AIController.h"
#include "Actors/Projectile/ProjectileBase.h"

void ASTGRIM::BeginPlay()
{
	Super::BeginPlay();

	USTGRIMAnimInstance* GrimAnimInst = Cast<USTGRIMAnimInstance>(GetMesh()->GetAnimInstance());
	if (GrimAnimInst)
	{
		// GrimAnimInst->Delegate_GrimPrimaryFire.AddUObject(this, &ASTGRIM::PrimaryFire);
		GrimAnimInst->Delegate_GrimUlitmateFire.AddUObject(this, &ASTGRIM::UltimateFire);
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

void ASTGRIM::PrimaryFire()
{
	if (HasAuthority())
	{
		FVector SpawnLocation = GetMesh()->GetSocketLocation(FName("Muzzle_01"));

		AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(SubclassOfPrimaryProjectile, SpawnLocation, FRotator::ZeroRotator);
		if (Projectile)
		{
			Projectile->FireInDirection(GetActorForwardVector());
		}
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
				FVector FireDirection = GetActorForwardVector();
				FRotator FireRotation = FireDirection.Rotation();
				FireRotation.Pitch += 67.5f;

				Projectile->FireInDirection(FireRotation.Vector());
				Projectile->SetHomingTarget(TargetActor);
			}
		}
	}
}
