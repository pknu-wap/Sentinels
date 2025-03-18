// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/STPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/SkillComponent.h"
#include "Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"

ASTPlayerCharacter::ASTPlayerCharacter()
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

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComp"));
	InventoryComponent->SetIsReplicated(true);

}

void ASTPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASTPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(Skill_Q_Action, ETriggerEvent::Started, this, &ASTPlayerCharacter::Skill_Q_Pressed);
		EnhancedInputComponent->BindAction(Skill_W_Action, ETriggerEvent::Started, this, &ASTPlayerCharacter::Skill_W_Pressed);
		EnhancedInputComponent->BindAction(Skill_E_Action, ETriggerEvent::Started, this, &ASTPlayerCharacter::Skill_E_Pressed);
		EnhancedInputComponent->BindAction(Skill_R_Action, ETriggerEvent::Started, this, &ASTPlayerCharacter::Skill_R_Pressed);

	}
}

void ASTPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASTPlayerCharacter, InventoryComponent);
}


#pragma region Region_Skills

void ASTPlayerCharacter::Skill_Q_Pressed()
{
	USkillComponent* SkillComp = GetController()->GetComponentByClass<USkillComponent>();
	if (SkillComp && SkillComp->CanActivateSkill(0))
	{
		SkillComp->ActivateSkill(0);

		PlayMontage_Skill_Q();
		Skill_Q_Pressed_Server();
	}
}

void ASTPlayerCharacter::PlayMontage_Skill_Q()
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->Montage_Play(Montage_Skill_Q);
	}
}

void ASTPlayerCharacter::Skill_Q_Pressed_Server_Implementation()
{
	Skill_Q_Pressed_Multicast();
}

void ASTPlayerCharacter::Skill_Q_Pressed_Multicast_Implementation()
{
	if (!IsLocallyControlled())
	{
		UE_LOG(LogTemp, Display, TEXT("ASTPlayerCharacter : Skill_Q_Pressed_Multicast Called!"));

		UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
		if (AnimInst)
		{
			AnimInst->Montage_Play(Montage_Skill_Q);
		}
	}
}

void ASTPlayerCharacter::Skill_W_Pressed()
{
	USkillComponent* SkillComp = GetController()->GetComponentByClass<USkillComponent>();
	if (SkillComp && SkillComp->CanActivateSkill(1))
	{
		SkillComp->ActivateSkill(1);

		PlayMontage_Skill_W();
		Skill_W_Pressed_Server();
	}
}

void ASTPlayerCharacter::PlayMontage_Skill_W()
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->Montage_Play(Montage_Skill_W);
	}
}

void ASTPlayerCharacter::Skill_W_Pressed_Server_Implementation()
{
	Skill_W_Pressed_Multicast();
}

void ASTPlayerCharacter::Skill_W_Pressed_Multicast_Implementation()
{
	if (!IsLocallyControlled())
	{
		UE_LOG(LogTemp, Display, TEXT("ASTPlayerCharacter : Skill_W_Pressed_Multicast Called!"));

		UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
		if (AnimInst)
		{
			AnimInst->Montage_Play(Montage_Skill_W);
		}
	}
}

void ASTPlayerCharacter::Skill_E_Pressed()
{
	USkillComponent* SkillComp = GetController()->GetComponentByClass<USkillComponent>();
	if (SkillComp && SkillComp->CanActivateSkill(2))
	{
		SkillComp->ActivateSkill(2);

		PlayMontage_Skill_E();
		Skill_E_Pressed_Server();
	}
}

void ASTPlayerCharacter::PlayMontage_Skill_E()
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->Montage_Play(Montage_Skill_E);
	}
}

void ASTPlayerCharacter::Skill_E_Pressed_Server_Implementation()
{
	Skill_E_Pressed_Multicast();
}

void ASTPlayerCharacter::Skill_E_Pressed_Multicast_Implementation()
{
	if (!IsLocallyControlled())
	{
		UE_LOG(LogTemp, Display, TEXT("ASTPlayerCharacter : Skill_E_Pressed_Multicast Called!"));

		UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
		if (AnimInst)
		{
			AnimInst->Montage_Play(Montage_Skill_E);
		}
	}
}

void ASTPlayerCharacter::Skill_R_Pressed()
{
	USkillComponent* SkillComp = GetController()->GetComponentByClass<USkillComponent>();
	if (SkillComp && SkillComp->CanActivateSkill(3))
	{
		SkillComp->ActivateSkill(3);

		PlayMontage_Skill_R();
		Skill_R_Pressed_Server();
	}
}

void ASTPlayerCharacter::PlayMontage_Skill_R()
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->Montage_Play(Montage_Skill_R);
	}
}

void ASTPlayerCharacter::Skill_R_Pressed_Server_Implementation()
{
	Skill_R_Pressed_Multicast();
}

void ASTPlayerCharacter::Skill_R_Pressed_Multicast_Implementation()
{
	if (!IsLocallyControlled())
	{
		UE_LOG(LogTemp, Display, TEXT("ASTPlayerCharacter : Skill_R_Pressed_Multicast Called!"));

		UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
		if (AnimInst)
		{
			AnimInst->Montage_Play(Montage_Skill_R);
		}
	}
}

#pragma endregion
