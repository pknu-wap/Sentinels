// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/STEnemyBase.h"
#include "Character/Enemy/STEnemyBase_AIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/STEnemyStatusComponent.h"
#include "Net/UnrealNetwork.h"
#include "Character/Enemy/STEnemyBase_AIController.h"
#include "SkeletalMeshComponentBudgeted.h"
#include "IAnimationBudgetAllocator.h"
#include "BrainComponent.h"
#include "DamageType/STDamageTypes.h"
#include "STGameplayTags.h"
#include "Character/STCharacterAnimInstanceBase.h"
#include "Actors/Projectile/ProjectileBase.h"
#include "Actors/Interact/Item/InteractableItem.h"
#include "SubSystem/STProjectilePoolingSubSystem.h"
#include "Particles/ParticleSystemComponent.h"

ASTEnemyBase::ASTEnemyBase(const FObjectInitializer& object_initializer)
	: Super(object_initializer.SetDefaultSubobjectClass<USkeletalMeshComponentBudgeted>(ACharacter::MeshComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	StatusComponent = CreateDefaultSubobject<USTEnemyStatusComponent>(TEXT("EnemyStatusComp"));
	StatusComponent->SetIsReplicated(true);

	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ASTEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	IAnimationBudgetAllocator::Get(GetWorld())->SetEnabled(true);

	if (HasAuthority())
	{
		if (USTCharacterAnimInstanceBase* AnimInst = Cast<USTCharacterAnimInstanceBase>(GetMesh()->GetAnimInstance()))
		{
			AnimInst->Delegate_PrimaryFire.AddUObject(this, &ASTEnemyBase::PrimaryFire);
			AnimInst->Delegate_DissolveStart.AddUObject(this, &ASTEnemyBase::DissolveStart);
		}

		Activate(GetActorLocation(), GetActorRotation());

		if (ProjectileClass_PrimaryFire)
		{
			USTProjectilePoolingSubSystem* ProjectileSubSystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<USTProjectilePoolingSubSystem>();
			ProjectileSubSystem->InitProjectilePool(this, ProjectileClass_PrimaryFire, 100);
		}
	}
}

void ASTEnemyBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASTEnemyBase, StatusComponent);
}

float ASTEnemyBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (StatusComponent && StatusComponent->IsDied())
		return 0.f;

	if (HasAuthority())
	{
		/*
			Critical
		*/
		if (DamageEvent.GetTypeID() == FSTPointDamageEvent::ClassID)
		{
			const FSTPointDamageEvent& PointDamageEvent = static_cast<const FSTPointDamageEvent&>(DamageEvent);
			FString Str_DamageType = PointDamageEvent.bIsCritical ? FString("Critical") : FString("Normal");
			UE_LOG(LogTemp, Warning, TEXT("ASTEnemyBase : %s Damage %f"), *Str_DamageType, Damage);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ASTEnemyBase : Damage %f"), Damage);
		}
		

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
				Apply DamageType
			*/
			if (STDamageType)
			{
				if (STDamageType->StunnedTime > 0)
				{
					// Apply Stun to Behavior Tree
					controller->ApplyStun(STDamageType->StunnedTime);

					// Apply Stun to Character (Animation)
					AddTag(FSTGameplayTags::Get().Character_State_Stunned);
					GetWorldTimerManager().SetTimer(Handle_Stunned,
						[&]() { RemoveTag(FSTGameplayTags::Get().Character_State_Stunned); },
						STDamageType->StunnedTime, false);
				}
			}

			if (KatanaDamageType)
			{
				AddTag(FSTGameplayTags::Get().Character_State_Bleed);
				UE_LOG(LogTemp, Display, TEXT("Katana Damage Type ! ! !"));
			}
		}

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

			// Delegate Broadcast
			Delegate_OnEnemyDied.Broadcast(this);

			// Drop Item
			DropItem();

			return ActualDamage;
		}

		/*
			Play Knockback Montage
		*/
		if (Montage_Knockback)
		{
			PlayKnockbackMontage_Multicast();
		}
		else
		{
			StopCurrentAnimMontage_Multicast();
			FVector LaunchDir = (GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal2D();
			LaunchCharacter(LaunchDir * 750.f, false, false);
		}
	}

	return 0.0f;
}

void ASTEnemyBase::Activate(const FVector ActivateLocation, const FRotator ActivateRotation)
{
	Super::Activate(ActivateLocation, ActivateRotation);

	DissolveReverseStart();
	StopAnimMontage();
	StatusComponent->InitStatus();
}

void ASTEnemyBase::Deactivate()
{
	Super::Deactivate();
}

bool ASTEnemyBase::IsNormalAttackMontage(UAnimMontage* InMontage)
{
	for (int i = 0; i < Montage_NormalAttackSet.Num(); i++)
	{
		if (Montage_NormalAttackSet[i] == InMontage)
			return true;
	}
	return false;
}

int ASTEnemyBase::GetRandomNormalAttackMontageIndex()
{
	const int32 NumMontages = Montage_NormalAttackSet.Num();

	if (NumMontages <= 1)
		return 0; // 하나뿐이면 어쩔 수 없음

	int32 NewIndex;
	do
	{
		NewIndex = FMath::RandRange(0, NumMontages - 1);
	} while (NewIndex == LastNormalAttackMontageIndex);

	LastNormalAttackMontageIndex = NewIndex;
	return NewIndex;
}

void ASTEnemyBase::StopCurrentAnimMontage_Multicast_Implementation()
{
	StopAnimMontage();
}

void ASTEnemyBase::ActivateNormalAttack_Server_Implementation()
{
	int MontageIdx = GetRandomNormalAttackMontageIndex();
	ActivateNormalAttack_Multicast(MontageIdx);
	PlayNormalAttackMontage(MontageIdx);
}

void ASTEnemyBase::ActivateNormalAttack_Multicast_Implementation(int MontageIdx)
{
	if(!HasAuthority())
		PlayNormalAttackMontage(MontageIdx);
}

void ASTEnemyBase::PlayNormalAttackMontage(int MontageIdx)
{
	if (!Montage_NormalAttackSet.IsValidIndex(MontageIdx))
		return;

	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->Montage_Play(Montage_NormalAttackSet[MontageIdx]);
	}
}

void ASTEnemyBase::PrimaryFire()
{
	USTProjectilePoolingSubSystem* ProjectileSubSystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<USTProjectilePoolingSubSystem>();

	FVector SpawnLocation = GetMesh()->GetSocketLocation(SocketName_PrimaryFire);
	AProjectileBase* projectile = ProjectileSubSystem->GetActor<AProjectileBase>(ProjectileClass_PrimaryFire, SpawnLocation);
	// AProjectileBase* projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass_PrimaryFire, SpawnLocation, GetActorForwardVector().Rotation());
	if (projectile)
	{
		UParticleSystemComponent* Particle = projectile->GetComponentByClass<UParticleSystemComponent>();
		if (Particle)
		{
			Particle->ActivateSystem(true);
		}
		projectile->FireInDirection(GetActorForwardVector());
	}
}

void ASTEnemyBase::PlayKnockbackMontage_Multicast_Implementation()
{
	PlayKnockbackMontage();
}

void ASTEnemyBase::PlayKnockbackMontage()
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->Montage_Play(Montage_Knockback);
	}
}

void ASTEnemyBase::DissolveEnded()
{
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().ClearTimer(Handle_Deactivate);
		Deactivate();
	}
}

void ASTEnemyBase::PlayDiedMontage_Multicast_Implementation()
{
	PlayDiedMontage();
}

void ASTEnemyBase::PlayDiedMontage()
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->Montage_Play(Montage_Died);
	}
}

void ASTEnemyBase::SetAdditionalDropInfos(const TArray<FDropInfo>& inDropInfos)
{
	DropInfos_Additional = inDropInfos;
}

void ASTEnemyBase::DropItem()
{
	if (HasAuthority())
	{
		float rand = UKismetMathLibrary::RandomFloatInRange(0, 1);

		if(DropInfo_Base.DropProbability >= rand)
		{
			GetWorld()->SpawnActor<AActor>(DropInfo_Base.DropItemClass, GetActorLocation(), GetActorRotation());
		}
		
		for (const auto& dropInfo : DropInfos_Additional)
		{
			rand = UKismetMathLibrary::RandomFloatInRange(0, 1);
			if (dropInfo.DropProbability >= rand)
			{
				GetWorld()->SpawnActor<AActor>(dropInfo.DropItemClass, GetActorLocation(), GetActorRotation());
			}
		}
		
	}
}

