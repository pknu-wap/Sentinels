// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/STPlayerCharacter.h"
#include "Player/STPlayerAnimInstance.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
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
#include "Components/STCharacterMovementComponent.h"
#include "Components/STPlayerStatusComponent.h"
#include "Components/CameraModeManagerComponent.h"
#include "Components/InteractComponent.h"
#include "Net/UnrealNetwork.h"
#include "STGameplayTags.h"

ASTPlayerCharacter::ASTPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USTCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
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

	StatusComponent = CreateDefaultSubobject<USTPlayerStatusComponent>(TEXT("StatusComp"));
	StatusComponent->SetIsReplicated(true);

	CameraManager = CreateDefaultSubobject<UCameraModeManagerComponent>(TEXT("CameraManager"));
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

	BindAttackDelegate();
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
		EnhancedInputComponent->BindAction(NormalAttack_Action, ETriggerEvent::Started, this, &ASTPlayerCharacter::NormalAttack_Pressed);
	}
}

void ASTPlayerCharacter::BindDefaultTopDownInput()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		if (UInputComponent* InputComp = PlayerController->GetComponentByClass<UInputComponent>())
		{
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComp))
			{
				EnhancedInputComponent->BindAction(NormalAttack_Action, ETriggerEvent::Started, this, &ASTPlayerCharacter::NormalAttack_Pressed);

				EnhancedInputComponent->BindAction(Skill_Q_Action, ETriggerEvent::Started, this, &ASTPlayerCharacter::Skill_Q_Pressed);
				EnhancedInputComponent->BindAction(Skill_W_Action, ETriggerEvent::Started, this, &ASTPlayerCharacter::Skill_W_Pressed);
				EnhancedInputComponent->BindAction(Skill_E_Action, ETriggerEvent::Started, this, &ASTPlayerCharacter::Skill_E_Pressed);
				EnhancedInputComponent->BindAction(Skill_R_Action, ETriggerEvent::Started, this, &ASTPlayerCharacter::Skill_R_Pressed);

				EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASTPlayerCharacter::Jump);
			}
		}
	}
}

void ASTPlayerCharacter::BindDefaultThirdPersonInput()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(MappingContext_ThirdPerson, 0);
		}

		if (UInputComponent* InputComp = PlayerController->GetComponentByClass<UInputComponent>())
		{
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComp))
			{
				EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASTPlayerCharacter::Move);
				EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASTPlayerCharacter::Look);

				EnhancedInputComponent->BindAction(NormalAttack_Action, ETriggerEvent::Started, this, &ASTPlayerCharacter::NormalAttack_Pressed);

				EnhancedInputComponent->BindAction(Skill_Q_Action, ETriggerEvent::Started, this, &ASTPlayerCharacter::Skill_Q_Pressed);
				EnhancedInputComponent->BindAction(Skill_W_Action, ETriggerEvent::Started, this, &ASTPlayerCharacter::Skill_W_Pressed);
				EnhancedInputComponent->BindAction(Skill_E_Action, ETriggerEvent::Started, this, &ASTPlayerCharacter::Skill_E_Pressed);
				EnhancedInputComponent->BindAction(Skill_R_Action, ETriggerEvent::Started, this, &ASTPlayerCharacter::Skill_R_Pressed);

				EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
				EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
			}
		}
	}
}

void ASTPlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASTPlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASTPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASTPlayerCharacter, InventoryComponent);
	DOREPLIFETIME(ASTPlayerCharacter, StatusComponent);
}

float ASTPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float actualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (HasTag(FSTGameplayTags::Get().Character_Player_State_RescueHostage))
	{
		// if damaged while Rescuing, Fail Interact
		UInteractComponent* IC = Cast<UInteractComponent>(GetController());
		if (IC)
		{
			IC->Interact_Finish_Server();
		}
	}

	return 0.0f;
}

#pragma region Region_NormalAttack

void ASTPlayerCharacter::BindAttackDelegate()
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->OnMontageEnded.AddDynamic(this, &ASTPlayerCharacter::OnMontageEnded_ResetAttackInfo);
	}

	USTPlayerAnimInstance* PAnimInst = Cast<USTPlayerAnimInstance>(AnimInst);
	if (PAnimInst)
	{
		PAnimInst->Delegate_StartCheckNextInput.AddUObject(this, &ASTPlayerCharacter::StartCheckNextInput);
		PAnimInst->Delegate_CheckNextAttack.AddUObject(this, &ASTPlayerCharacter::CheckNextAttack);
	}
}

void ASTPlayerCharacter::NormalAttack_Pressed()
{
	if (CurrentCombo == 0)
	{
		CurrentCombo++;
		PlayMontage_NormalAttack(CurrentCombo);
		NormalAttack_Pressed_Server(CurrentCombo);
	}

	else if (bIsCheckingNextInput)
	{
		bShouldDoNextAttack = true;
	}
}

void ASTPlayerCharacter::PlayMontage_NormalAttack(int currentCombo)
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		if (currentCombo <= 1)
		{
			AnimInst->Montage_Play(Montage_NormalAttack);
		}
		else
		{
			FString currentSection = GetMesh()->GetAnimInstance()->Montage_GetCurrentSection().ToString();
			currentSection[currentSection.Len() - 1] += 1;
			AnimInst->Montage_JumpToSection(FName(currentSection));
		}
	}
}

void ASTPlayerCharacter::NormalAttack_Pressed_Server_Implementation(int currentCombo)
{
	NormalAttack_Pressed_Multicast(currentCombo);
}

void ASTPlayerCharacter::NormalAttack_Pressed_Multicast_Implementation(int currentCombo)
{
	if (!IsLocallyControlled())
	{
		PlayMontage_NormalAttack(currentCombo);
	}
}

void ASTPlayerCharacter::StartCheckNextInput()
{
	bIsCheckingNextInput = true;
}

void ASTPlayerCharacter::CheckNextAttack()
{
	if (!bShouldDoNextAttack || CurrentCombo > MaxCombo)
	{
		ResetAttackInfo();
	}
	else if (bShouldDoNextAttack)
	{
		bIsCheckingNextInput = false; bShouldDoNextAttack = false; 
		CurrentCombo++;

		PlayMontage_NormalAttack(CurrentCombo);
		NormalAttack_Pressed_Server(CurrentCombo);
	}
}

void ASTPlayerCharacter::OnMontageEnded_ResetAttackInfo(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted && Montage == Montage_NormalAttack)
		return;

	ResetAttackInfo();
}

void ASTPlayerCharacter::ResetAttackInfo()
{
	CurrentCombo = 0; 
	bShouldDoNextAttack = false;
	bIsCheckingNextInput = false;
}

#pragma endregion


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
		PlayMontage_Skill_Q();
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
		PlayMontage_Skill_W();
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
		PlayMontage_Skill_E();
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
		PlayMontage_Skill_R();
	}
}

#pragma endregion
