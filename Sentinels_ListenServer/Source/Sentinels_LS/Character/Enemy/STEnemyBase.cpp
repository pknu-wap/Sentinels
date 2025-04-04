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
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
			if (BBComp)
			{
				BBComp->SetValueAsVector(FName("StartLocation"), GetActorLocation());
			}
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

	ASTEnemyBase_AIController* controller = Cast<ASTEnemyBase_AIController>(GetController());
	if (controller)
	{
		controller->SetTarget(DamageCauser);
	}

	return 0.0f;
}

void ASTEnemyBase::ActivateNormalAttack_Server_Implementation()
{
	ActivateNormalAttack_Multicast();
	PlayNormalAttackMontage();
}

void ASTEnemyBase::ActivateNormalAttack_Multicast_Implementation()
{
	PlayNormalAttackMontage();
}

void ASTEnemyBase::PlayNormalAttackMontage()
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->Montage_Play(Montage_NormalAttack);
	}
}
