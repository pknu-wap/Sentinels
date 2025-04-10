// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/GRIM/STGRIM.h"
#include "Character/Enemy/GRIM/STGRIMAnimInstance.h"
#include "Actors/Projectile/ProjectileBase.h"

void ASTGRIM::BeginPlay()
{
	Super::BeginPlay();

	USTGRIMAnimInstance* GrimAnimInst = Cast<USTGRIMAnimInstance>(GetMesh()->GetAnimInstance());
	if (GrimAnimInst)
	{
		GrimAnimInst->Delegate_GrimPrimaryFire.AddUObject(this, &ASTGRIM::PrimaryFire);
	}

}

void ASTGRIM::PrimaryFire()
{
	FVector SpawnLocation = GetMesh()->GetSocketLocation(FName("Muzzle_01"));

	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(SubclassOfPrimaryProjectile, SpawnLocation, FRotator::ZeroRotator);
	if (Projectile)
	{
		Projectile->FireInDirection(GetActorForwardVector());
	}
}
