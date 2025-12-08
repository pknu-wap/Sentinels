// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/GRIM/STGRIM.h"
#include "Character/Enemy/GRIM/STGRIMAnimInstance.h"
#include "Character/Enemy/STEnemyBase_AIController.h"
#include "Actors/Projectile/ProjectileBase.h"

void ASTGRIM::BeginPlay()
{
	Super::BeginPlay();

	USTGRIMAnimInstance* AnimInst = Cast<USTGRIMAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInst)
	{
		AnimInst->Delegate_UltimateFire.AddUObject(this, &ASTGRIM::UltimateFire);
		AnimInst->Delegate_UltimateFire_Red.AddUObject(this, &ASTGRIM::UltimateFire_Red);
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
		ASTEnemyBase_AIController* controller = Cast<ASTEnemyBase_AIController>(GetController());

		if (controller)
		{
			AActor* TargetPlayer = controller->GetCurrentTarget();
			if (!TargetPlayer) return;

			UltimateFire_Multicast(TargetPlayer);
		}
	}
}

void ASTGRIM::UltimateFire_Multicast_Implementation(AActor* TargetPlayer)
{
	if (!TargetPlayer) return;

	FVector SpawnLocation = GetMesh()->GetSocketLocation(FName("Muzzle_04"));
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

void ASTGRIM::UltimateFire_Red()
{
	if (HasAuthority())
	{
		ASTEnemyBase_AIController* controller = Cast<ASTEnemyBase_AIController>(GetController());

		if (controller)
		{
			AActor* TargetPlayer = controller->GetCurrentTarget();
			if (!TargetPlayer) return;

			UltimateFire_Red_Multicast(TargetPlayer->GetActorLocation());
		}
	}
}

void ASTGRIM::UltimateFire_Red_Multicast_Implementation(FVector TargetLocation)
{
	FVector SpawnLocation = GetMesh()->GetSocketLocation(FName("Muzzle_04"));

	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(SubclassOfUltimateProjectile, SpawnLocation, FRotator::ZeroRotator);
	if (Projectile)
	{
		Projectile->SetInstigator(this);

		FVector FireDirection = GetActorForwardVector();
		FRotator FireRotation = FireDirection.Rotation();

		Projectile->FireInDirection(FireRotation.Vector());
	}
}
