// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Crunch/STCrunch.h"
#include "Actors/Projectile/ProjectileBase.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

void ASTCrunch::StartSpawnMissiles()
{
	if (HasAuthority())
	{
		PlayAnimMontage(Montage_SpawnMissile);
		GetWorldTimerManager().SetTimer(Handle_SpawnMissiles, this, &ASTCrunch::SpawnMissile, 0.1f, true);
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
