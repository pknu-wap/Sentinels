// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Helix/STHelix.h"
#include "Character/Enemy/Helix/STHelixAnimInstance.h"
#include "Actors/Projectile/ProjectileBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/STEnemyStatusComponent.h"
#include "STGameplayTags.h"
#include "Character/Enemy/STEnemyBase_AIController.h"
#include "BrainComponent.h"

void ASTHelix::BeginPlay()
{
	Super::BeginPlay();
}

float ASTHelix::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HasAuthority())
	{
		if (StatusComponent && StatusComponent->IsDied())
			return 0.f;

		/*
			Set Target & Apply DamageType
		*/
		ASTEnemyBase_AIController* controller = Cast<ASTEnemyBase_AIController>(GetController());
		USTBaseDamageType* STDamageType = Cast<USTBaseDamageType>(DamageEvent.DamageTypeClass.GetDefaultObject());
		USTKatanaDamageType* KatanaDamageType = Cast<USTKatanaDamageType>(DamageEvent.DamageTypeClass.GetDefaultObject());
		if (controller)
		{
			/*
				Set Target as DamageCauser
			*/
			controller->SetTarget(DamageCauser);

			/*
				Critical
			*/
			FLinearColor damageTextColor = FLinearColor::White;
			if (DamageEvent.GetTypeID() == FSTPointDamageEvent::ClassID)
			{
				const FSTPointDamageEvent& PointDamageEvent = static_cast<const FSTPointDamageEvent&>(DamageEvent);
				FString Str_DamageType = PointDamageEvent.bIsCritical ? FString("Critical") : FString("Normal");
				damageTextColor = PointDamageEvent.bIsCritical ? FLinearColor::Yellow : FLinearColor::White;
				UE_LOG(LogTemp, Warning, TEXT("ASTEnemyBase : %s Damage %f"), *Str_DamageType, Damage);
			}
			else
			{
				damageTextColor = FLinearColor::White;
				UE_LOG(LogTemp, Warning, TEXT("ASTEnemyBase : Extra Damage %f"), Damage);
			}

			/*
				Apply DamageType
			*/
			if (STDamageType)
			{
				if (STDamageType->StunnedTime > 0)
				{
					// Apply Stun to Behavior Tree
					controller->ApplyStun(STDamageType->StunnedTime);

					// Apply Stun to Character (Animation)
					if (!HasTag(FSTGameplayTags::Get().Character_State_Stunned))
						UpdateEnemyStateWidget_Multicast(FSTGameplayTags::Get().Character_State_Stunned, true);

					AddTag(FSTGameplayTags::Get().Character_State_Stunned);
					GetWorldTimerManager().SetTimer(Handle_Stunned,
						[&]() {
							ClearTag(FSTGameplayTags::Get().Character_State_Stunned);
							UpdateEnemyStateWidget_Multicast(FSTGameplayTags::Get().Character_State_Stunned, false);
						},
						STDamageType->StunnedTime, false);
				}
			}

			if (KatanaDamageType)
			{
				AddTag(FSTGameplayTags::Get().Character_State_Bleed);
				Delegate_OnEnemyStateAdd.Broadcast(FSTGameplayTags::Get().Character_State_Bleed);
				UE_LOG(LogTemp, Display, TEXT("Katana Damage Type ! ! !"));
			}

			/*
			Damage Indicate
			*/
			ShowDamageIndicateWidget_Multicast(Damage, damageTextColor);

			/*
				Calculate Current HP & Check Died
			*/
			if (StatusComponent && StatusComponent->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser) <= 0)
			{
				// Play Died Montage
				GetWorld()->GetTimerManager().SetTimer(Handle_Deactivate,
					[this]()
					{
						this->Deactivate();
					}, 5.f, false);

				PlayDiedMontage_Multicast();

				// Stop Behavior Tree
				AAIController* AIController = Cast<AAIController>(GetController());
				if (AIController && AIController->GetBrainComponent())
				{
					AIController->GetBrainComponent()->StopLogic(FString("Died."));
				}

				// Drop Item
				DropItem();

				return Damage;
			}
		}
	}

	return 0.0f;
}

void ASTHelix::HelixAttack(EHelixAttackType AttackType)
{
	switch (AttackType)
	{
	case EHelixAttackType::Special_01:
	{
		PlayAnimMontage(AM_Special_01);
		break;
	}
	case EHelixAttackType::Special_02:
	{
		PlayAnimMontage(AM_Special_02);
		break;
	}
	case EHelixAttackType::Special_03:
	{
		PlayAnimMontage(AM_Special_03);
		break;
	}
	case EHelixAttackType::Cast:
	{
		PlayAnimMontage(AM_Cast);
		break;
	}
	default:
		break;
	}
}

void ASTHelix::StartSpawnLazers()
{
	if (HasAuthority())
	{
		// Clear key values
		CurrentLazerCount = 0;

		// Set timer for spawn
		SpawnLazers();
		// GetWorldTimerManager().SetTimer(Handle_Lazer, this, &ASTHelix::SpawnLazers, 1, true, 0.f);
	}
}

void ASTHelix::SpawnLazers()
{
	// Spawn Lazer On Chest (It need Decal? and How to Show Decal?)
	FActorSpawnParameters spawnParam;
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParam.Owner = this; spawnParam.Instigator = this;

	FVector spawnLocation = GetMesh()->GetSocketLocation(FName("Muzzle_02"));
	FRotator spawnRotator = GetMesh()->GetSocketRotation(FName("Muzzle_02")) + FRotator(-20, 0, 0);
	AActor* lazer = GetWorld()->SpawnActor<AActor>(LazerActorClass, spawnLocation, spawnRotator, spawnParam);



	// add counter and Clear Timer if spawn over max count
	CurrentLazerCount++;
	if (CurrentLazerCount >= MaxLazerCount)
	{
		GetWorldTimerManager().ClearTimer(Handle_Lazer);
	}
}

void ASTHelix::StartSpawnProjectiles()
{
	if (HasAuthority())
	{
		// Clear key values
		SpawnedProjectiles = 0;

		// Set timer for spawn
		GetWorldTimerManager().SetTimer(Handle_Projectiles, this, &ASTHelix::SpawnProjectile, 2.f / MaxSpawnProjectiles, true, 0.f);
	}
}

void ASTHelix::SpawnProjectile()
{
	if (UBlackboardComponent* blackboard = GetController()->GetComponentByClass<UBlackboardComponent>())
	{
		if (AActor* target = Cast<AActor>(blackboard->GetValueAsObject(FName("Target"))))
		{
			// Spawn Projectiles On Chest
			FActorSpawnParameters spawnParam;
			spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			spawnParam.Owner = this; spawnParam.Instigator = this;

			float angleOffset = 360.f / MaxSpawnProjectiles;

			FVector spawnLocation = GetActorLocation() + UKismetMathLibrary::RotateAngleAxis(FVector(400, 0, 0), angleOffset * SpawnedProjectiles, FVector(0, 0, 1)) - FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

			// UE_LOG(LogTemp, Display, TEXT("%f, %f, %s"), angleOffset * SpawnedProjectiles, SpawnedProjectiles, *UKismetMathLibrary::RotateAngleAxis(FVector(200, 0, 0), angleOffset * SpawnedProjectiles, FVector(0, 0, 1)).ToString());
			// UE_LOG(LogTemp, Display, TEXT("%s"), *spawnLocation.ToString());
			// DrawDebugSphere(GetWorld(), spawnLocation, 50, 16, FColor::Red, false, 3.f);

			AProjectileBase* projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, spawnLocation, FRotator::ZeroRotator, spawnParam);
			if (projectile)
			{
				projectile->SetHomingTarget(target);
				projectile->FireInDirection(FVector(0, 0, 1));
			}

			// add counter and Clear Timer if spawn over max count
			SpawnedProjectiles++;
			if (SpawnedProjectiles >= MaxSpawnProjectiles)
			{
				GetWorldTimerManager().ClearTimer(Handle_Projectiles);
			}
		}
	}
}

void ASTHelix::SpawnShockWave()
{
	if (HasAuthority())
	{
		FActorSpawnParameters spawnParam;
		spawnParam.Owner = this; spawnParam.Instigator = this; 

		FVector heightOffset = FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		GetWorld()->SpawnActor<AActor>(ShockWaveActorClass, GetActorLocation() + heightOffset, FRotator::ZeroRotator, spawnParam);
	}
}

void ASTHelix::StartSpawnExplosions()
{
	if (HasAuthority())
	{
		// Clear key values
		SpawnedExplosions = 0;

		// Set timer for spawn
		GetWorldTimerManager().SetTimer(Handle_Explosion, this, &ASTHelix::SpawnExplosion, 0.25, true, 0.f);
	}
}

void ASTHelix::SpawnExplosion()
{
	if (UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem()))
	{
		FNavLocation navResultLocation;
		
		if (NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), 3000.f, navResultLocation))
		{
			FActorSpawnParameters spawnParam;
			spawnParam.Owner = this; spawnParam.Instigator = this;

			// Spawn Explosion On Random Location
			GetWorld()->SpawnActor<AActor>(ExplosionClass, navResultLocation.Location, FRotator::ZeroRotator, spawnParam);

			// add counter and Clear Timer if spawn over max count
			SpawnedExplosions++;
			if (SpawnedExplosions >= MaxSpawnExplosions)
			{
				GetWorldTimerManager().ClearTimer(Handle_Explosion);
			}
		}
	}
}
