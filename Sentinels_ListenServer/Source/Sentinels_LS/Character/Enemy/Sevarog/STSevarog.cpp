// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Sevarog/STSevarog.h"
#include "Character/Enemy/STEnemyBase_AIController.h"
#include "Character/STCharacterAnimInstanceBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

void ASTSevarog::BeginPlay()
{
	Super::BeginPlay();

	AnimInst = Cast<USTCharacterAnimInstanceBase>(GetMesh()->GetAnimInstance());
	if (AnimInst)
	{
		AnimInst->Delegate_UltimateFire.AddUObject(this, &ASTSevarog::UltimateFireCallback);
	}
}

void ASTSevarog::UltimateFireCallback()
{
	if (AnimInst)
	{
		UAnimMontage* currentMontage = AnimInst->GetCurrentActiveMontage();

		// Subjection
		if (Montage_EliteAttackSet.IsValidIndex(0) && currentMontage == Montage_EliteAttackSet[0])
		{
			SpawnSubjection();
		}
		// SoulSiphon
		else if (Montage_EliteAttackSet.IsValidIndex(1) && currentMontage == Montage_EliteAttackSet[1])
		{
			StartSoulSiphon();
		}
		// AOE
		else if (Montage_EliteAttackSet.IsValidIndex(2) && currentMontage == Montage_EliteAttackSet[2])
		{
			SpawnAOE();
		}
	}
}

void ASTSevarog::StartSoulSiphon()
{
	if (HasAuthority() && Controller)
	{
		EnemyAIController = EnemyAIController == nullptr ? Cast<ASTEnemyBase_AIController>(Controller) : EnemyAIController;
		if (!EnemyAIController) return;

		// Target Should not be changed while SoulSiphoning!
		SoulSiphonTarget = EnemyAIController->GetCurrentTarget();
		if (SoulSiphonTarget)
		{
			SoulSiphonCount = 0;

			FTimerHandle handle;
			GetWorldTimerManager().SetTimer(handle, this, &ASTSevarog::ApplySoulSiphonDamage, 0.1f, true);
		}
	}
}

void ASTSevarog::ApplySoulSiphonDamage()
{
	if (SoulSiphonTarget)
	{
		UGameplayStatics::ApplyDamage(SoulSiphonTarget, 5.f, GetController(), this, UDamageType::StaticClass());
	}

	SoulSiphonCount++;
	if(SoulSiphonCount > 30)
		GetWorldTimerManager().ClearTimer(SoulSiphonHandle);
}

void ASTSevarog::SpawnSubjection()
{
	if (HasAuthority() && Controller)
	{
		EnemyAIController = EnemyAIController == nullptr ? Cast<ASTEnemyBase_AIController>(Controller) : EnemyAIController;
		if (!EnemyAIController) return;
		
		AActor* currentTarget = EnemyAIController->GetCurrentTarget();
		if (currentTarget)
		{
			FActorSpawnParameters spawnParam;
			spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			spawnParam.Owner = this; spawnParam.Instigator = this;

			UCapsuleComponent* capsuleComp = currentTarget->GetComponentByClass<UCapsuleComponent>();
			if(capsuleComp)
				GetWorld()->SpawnActor<AActor>(SubjectionActorClass, 
					currentTarget->GetActorLocation() - FVector(0.f, 0.f, capsuleComp->GetScaledCapsuleHalfHeight()), FRotator::ZeroRotator, spawnParam);
		}
	}
}

void ASTSevarog::SpawnAOE()
{
	if (HasAuthority())
	{
		FActorSpawnParameters spawnParam;
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		spawnParam.Owner = this; spawnParam.Instigator = this;

		for (int i = 0; i < 3; i++)
		{
			FVector unitVector = UKismetMathLibrary::RandomUnitVector(); unitVector.Z = 0;
			unitVector.Normalize();
			GetWorld()->SpawnActor<AActor>(AOEActorClass,
				GetActorLocation() + unitVector * 500 - GetCapsuleComponent()->GetScaledCapsuleHalfHeight(), FRotator::ZeroRotator, spawnParam);
		}
	}
}
