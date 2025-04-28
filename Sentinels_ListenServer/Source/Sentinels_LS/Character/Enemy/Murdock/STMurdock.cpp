// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Murdock/STMurdock.h"
#include "Character/STCharacterAnimInstanceBase.h"
#include "Actors/Projectile/ProjectileBase.h"

void ASTMurdock::BeginPlay()
{
	Super::BeginPlay();

	USTCharacterAnimInstanceBase* AnimInst = Cast<USTCharacterAnimInstanceBase>(GetMesh()->GetAnimInstance());
	if (AnimInst)
	{
		AnimInst->Delegate_UltimateFire.AddUObject(this, &ASTMurdock::UltimateFire);
	}
}

void ASTMurdock::ActivateUltimate_Server_Implementation()
{
	ActivateUltimate_Multicast();
	PlayUltimateMontage();
}

void ASTMurdock::ActivateUltimate_Multicast_Implementation()
{
	PlayUltimateMontage();
}

void ASTMurdock::PlayUltimateMontage()
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->Montage_Play(Montage_Ultimate);
	}
}

void ASTMurdock::UltimateFire()
{
	if (HasAuthority())
	{
		FVector SpawnLocation = GetMesh()->GetSocketLocation(FName("Muzzle_01"));

		AProjectileBase* projectile = GetWorld()->SpawnActor<AProjectileBase>(SubclassOfUltimateProjectile, SpawnLocation, GetActorForwardVector().Rotation());
		if (projectile)
		{
			projectile->FireInDirection(GetActorForwardVector());
		}
	}
}
