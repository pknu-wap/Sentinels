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
#include "Components/STEnemyStatusComponent.h"
#include "Net/UnrealNetwork.h"
#include "STGameplayTags.h"
#include "Player/Inventory/ItemObject.h"
#include "MotionWarpingComponent.h"
#include "SubSystem/STGameTravelDataSubsystem.h"
#include "GameFramework/PlayerState.h"

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

	BindAttackDelegate();
	if (USTPlayerAnimInstance* PAnimInst = Cast<USTPlayerAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		PAnimInst->Delegate_SetWarpTarget_Step.AddUObject(this, &ASTPlayerCharacter::SetWarpTarget_Step);
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
		EnhancedInputComponent->BindAction(Skill_Passive_Action, ETriggerEvent::Started, this, &ASTPlayerCharacter::Skill_Passive_Pressed);
		EnhancedInputComponent->BindAction(NormalAttack_Action, ETriggerEvent::Started, this, &ASTPlayerCharacter::NormalAttack_Pressed);
		EnhancedInputComponent->BindAction(NormalAttack_Action, ETriggerEvent::Completed, this, &ASTPlayerCharacter::NormalAttack_Released);
		EnhancedInputComponent->BindAction(Step_Action, ETriggerEvent::Started, this, &ASTPlayerCharacter::Step_Pressed);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASTPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASTPlayerCharacter::Look);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &ASTPlayerCharacter::Run_Pressed);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ASTPlayerCharacter::Run_Released);
	}
}

void ASTPlayerCharacter::ClearAllMappingContext()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
		}
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

void ASTPlayerCharacter::Run_Pressed()
{
	Run_Pressed_Server();
}

void ASTPlayerCharacter::Run_Released()
{
	// Run_Released_Server();
}

void ASTPlayerCharacter::Run_Pressed_Server_Implementation()
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (StatusComponent && MoveComp)
	{
		if (MoveComp->MaxWalkSpeed == StatusComponent->MovementSpeed)
		{
			MoveComp->MaxWalkSpeed = StatusComponent->MovementSpeed * 1.5;
		}
		else
		{
			MoveComp->MaxWalkSpeed = StatusComponent->MovementSpeed;
		}
	}
}

void ASTPlayerCharacter::Run_Released_Server_Implementation()
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (StatusComponent && MoveComp)
	{
		MoveComp->MaxWalkSpeed = StatusComponent->MovementSpeed;
	}
}

void ASTPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASTPlayerCharacter, InventoryComponent);
	DOREPLIFETIME(ASTPlayerCharacter, StatusComponent);
	DOREPLIFETIME(ASTPlayerCharacter, SKMeshPartsName);
}

float ASTPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float actualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (HasAuthority())
	{
		if (HasTag(FSTGameplayTags::Get().Character_Player_State_RescueHostage) || HasTag(FSTGameplayTags::Get().Character_Player_State_RepairRift))
		{
			// if damaged while Rescuing, Fail Interact

			AController* controller = GetController();
			if (controller)
			{
				UInteractComponent* IC = controller->GetComponentByClass<UInteractComponent>();
				if (IC)
				{
					IC->Interact_Finish_Server();
				}
			}
		}

		if (InventoryComponent)
		{
			const TArray<FInvSlotStruct>& inventory = InventoryComponent->GetInventory();
			for (const auto& item : inventory)
			{
				if (item.ItemObject)
				{
					actualDamage = item.ItemObject->OnDamaged(actualDamage, DamageEvent, EventInstigator, DamageCauser);
				}
			}
		}

		if (StatusComponent)
		{
			float HP = StatusComponent->TakeDamage(actualDamage, DamageEvent, EventInstigator, DamageCauser);
			if (HP <= 0)
			{
				if (APlayerController* PC = Cast<APlayerController>(GetController()))
				{
					PC->ServerRestartPlayer();
				}
			}
		}

	}

	return 0.0f;
}

void ASTPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsLocallyControlled())
	{
		//Add Input Mapping Context
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			if (USkillComponent* SkillComp = PlayerController->GetComponentByClass<USkillComponent>())
			{
				SkillComp->InitSkillInfos(DataTable_Skill);
			}
		}

		BindDefaultThirdPersonInput();
		CameraManager->InitCameraMode();

		TagContainer.Reset();
	}

	if (HasAuthority())
	{
		USTGameTravelDataSubsystem* gameTravelDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USTGameTravelDataSubsystem>();

		FPlayerSKMeshesRowName sKMeshesRowName = gameTravelDataSubsystem->LoadPlayerInfo(GetController()->PlayerState->GetUniqueId()).PlayerSKMeshesRowName;
		ConvertRowNametoPartsName(sKMeshesRowName);
	}

	OnRep_SKMeshPartsName();
}

void ASTPlayerCharacter::Jump()
{
	if (HasTag(FSTGameplayTags::Get().Character_State_Attack) 
		|| HasTag(FSTGameplayTags::Get().Character_State_Skill)) return;

	Super::Jump();
}

void ASTPlayerCharacter::OnMontageEnded_Callback(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		ClearTag(FSTGameplayTags::Get().Character_State_Skill);
		ClearTag(FSTGameplayTags::Get().Character_State_Step);
		ClearTag(FSTGameplayTags::Get().Character_State_Attack);
		ResetAttackInfo();
	}
	else if(bInterrupted && IsSkillMontage(GetCurrentMontage()))
	{
		AddUniqueTag(FSTGameplayTags::Get().Character_State_Skill);
	}
}

#pragma region Region_NormalAttack

void ASTPlayerCharacter::BindAttackDelegate()
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->OnMontageEnded.AddDynamic(this, &ASTPlayerCharacter::OnMontageEnded_Callback);
	}

	USTPlayerAnimInstance* PAnimInst = Cast<USTPlayerAnimInstance>(AnimInst);
	if (PAnimInst)
	{
		PAnimInst->Delegate_StartCheckNextInput.AddUObject(this, &ASTPlayerCharacter::StartCheckNextInput);
		PAnimInst->Delegate_CheckNextAttack.AddUObject(this, &ASTPlayerCharacter::CheckNextAttack);
	}
}

bool ASTPlayerCharacter::CanDoAttack() const
{
	if (HasTag(FSTGameplayTags::Get().Character_State_Skill) 
		|| HasTag(FSTGameplayTags::Get().Character_State_Jump)
		|| HasTag(FSTGameplayTags::Get().Character_State_Step))
		return false;

	return true;
}

void ASTPlayerCharacter::NormalAttack_Pressed()
{
	bIsAttackPressinng = true;

	if (!CanDoAttack()) return;

	AddUniqueTag(FSTGameplayTags::Get().Character_State_Attack);

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

void ASTPlayerCharacter::NormalAttack_Released()
{
	bIsAttackPressinng = false;
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
	if (bIsAttackPressinng || (bShouldDoNextAttack && CurrentCombo <= MaxCombo))
	{
		bIsCheckingNextInput = false; bShouldDoNextAttack = false;
		CurrentCombo++;

		PlayMontage_NormalAttack(CurrentCombo);
		NormalAttack_Pressed_Server(CurrentCombo);
	}
	else
	{
		ResetAttackInfo();
	}

	/*if (!bShouldDoNextAttack || CurrentCombo > MaxCombo)
	{
		ResetAttackInfo();
	}
	else if (bShouldDoNextAttack)
	{
		bIsCheckingNextInput = false; bShouldDoNextAttack = false; 
		CurrentCombo++;

		PlayMontage_NormalAttack(CurrentCombo);
		NormalAttack_Pressed_Server(CurrentCombo);
	}*/
}

void ASTPlayerCharacter::ResetAttackInfo()
{
	CurrentCombo = 0; 
	bShouldDoNextAttack = false;
	bIsCheckingNextInput = false;
}

#pragma endregion

void ASTPlayerCharacter::ApplyCustomTimeDilation_Implementation(float inValue, float inDuration)
{
	CustomTimeDilation = inValue;

	GetWorldTimerManager().SetTimer(Handle_TimeDilation, 
		[this]() 
		{ this->CustomTimeDilation = 1.f; },
		inDuration, false);
}

void ASTPlayerCharacter::ApplyAttackCameraShake_Implementation()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->ClientStartCameraShake(CameraShakeClass_Attack, 1.f);
	}
}

#pragma region Region_Step

bool ASTPlayerCharacter::CanDoStep() const
{
	/* || HasTag(FSTGameplayTags::Get().Character_State_Skill)
	|| HasTag(FSTGameplayTags::Get().Character_State_Attack)*/

	if (HasTag(FSTGameplayTags::Get().Character_State_Step)
		|| HasTag(FSTGameplayTags::Get().Character_State_Jump))
		return false;

	return true;
}

void ASTPlayerCharacter::Step_Pressed()
{
	if (!CanDoStep()) return;

	Step_Pressed_Server();
	PlayMontage_Step();
	AddUniqueTag(FSTGameplayTags::Get().Character_State_Step);
}

void ASTPlayerCharacter::PlayMontage_Step()
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->Montage_Play(Montage_Step_F);
	}
}

void ASTPlayerCharacter::SetWarpTarget_Step()
{
	if (!HasAuthority()) return;

	if (UMotionWarpingComponent* motionWarpComp = GetComponentByClass<UMotionWarpingComponent>())
	{
		FMotionWarpingTarget WarpTarget;
		WarpTarget.Name = FName("WarpTarget");
		WarpTarget.Location = GetActorLocation() + GetActorForwardVector() * StepDistance;
		WarpTarget.Rotation = GetControlRotation();

		motionWarpComp->AddOrUpdateWarpTarget(WarpTarget);
	}
}

void ASTPlayerCharacter::Step_Pressed_Server_Implementation()
{
	Step_Pressed_Multicast();
}

void ASTPlayerCharacter::Step_Pressed_Multicast_Implementation()
{
	if (!IsLocallyControlled())
	{
		PlayMontage_Step();
	}
}

#pragma endregion



#pragma region Region_Skills

bool ASTPlayerCharacter::CanDoSkill() const
{
	if (HasTag(FSTGameplayTags::Get().Character_State_Skill)
		|| HasTag(FSTGameplayTags::Get().Character_State_Jump)
		|| HasTag(FSTGameplayTags::Get().Character_State_Step))
		return false;

	return true;
}

bool ASTPlayerCharacter::IsSkillMontage(const UAnimMontage* inMontage) const
{
	return inMontage == Montage_Skill_Q 
			|| inMontage == Montage_Skill_W 
			|| inMontage == Montage_Skill_E 
			|| inMontage == Montage_Skill_R
			|| inMontage == Montage_Skill_Passive;
}

void ASTPlayerCharacter::Skill_Q_Pressed()
{
	if (!CanDoSkill()) return;

	USkillComponent* SkillComp = GetController()->GetComponentByClass<USkillComponent>();
	if (SkillComp && SkillComp->CanActivateSkill(0))
	{
		SkillComp->ActivateSkill(0);

		AddUniqueTag(FSTGameplayTags::Get().Character_State_Skill);
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
	if (!CanDoSkill()) return;

	USkillComponent* SkillComp = GetController()->GetComponentByClass<USkillComponent>();
	if (SkillComp && SkillComp->CanActivateSkill(1))
	{
		SkillComp->ActivateSkill(1);

		AddUniqueTag(FSTGameplayTags::Get().Character_State_Skill);
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
	if (!CanDoSkill()) return;

	USkillComponent* SkillComp = GetController()->GetComponentByClass<USkillComponent>();
	if (SkillComp && SkillComp->CanActivateSkill(2))
	{
		SkillComp->ActivateSkill(2);

		AddUniqueTag(FSTGameplayTags::Get().Character_State_Skill);
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
	if (!CanDoSkill()) return;

	USkillComponent* SkillComp = GetController()->GetComponentByClass<USkillComponent>();
	if (SkillComp && SkillComp->CanActivateSkill(3))
	{
		SkillComp->ActivateSkill(3);

		AddUniqueTag(FSTGameplayTags::Get().Character_State_Skill);
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

void ASTPlayerCharacter::Skill_Passive_Pressed()
{
}

void ASTPlayerCharacter::PlayMontage_Skill_Passive()
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->Montage_Play(Montage_Skill_Passive);
	}
}

void ASTPlayerCharacter::Skill_Passive_Pressed_Server_Implementation()
{
}

void ASTPlayerCharacter::Skill_Passive_Pressed_Multicast_Implementation()
{
	if (!IsLocallyControlled())
	{
		PlayMontage_Skill_Passive();
	}
}

#pragma endregion

void ASTPlayerCharacter::ApplyDamageToActor(float DamagePercent, TSubclassOf<UDamageType> DamageType, AActor* DamagedActor)
{
	FSTPointDamageEvent DamageEvent;

	FSTDamageInfo DamageInfo = StatusComponent->GetCalculatedDamageInfo(DamageEvent, DamagedActor);
	float damage = DamageInfo.DamageAmount * DamagePercent;

	DamageEvent.bIsCritical = DamageInfo.bIsCritical;
	DamageEvent.DamageTypeClass = DamageType;

	// Apply Damage
	DamagedActor->TakeDamage(damage, DamageEvent, GetController(), this);

	// Player Logic Execute (After)
	OnAttackSuccess_Server(damage, DamageEvent, DamagedActor);
}

void ASTPlayerCharacter::AdjustFinalDamage(float& DamageAmount, FDamageEvent const& DamageEvent, AActor* DamagedActor)
{
	if (!HasAuthority()) return;
}

void ASTPlayerCharacter::OnRep_SKMeshPartsName()
{
	FPlayerSKMeshesRowName rowName;
	ConvertPartsNametoRowName(rowName);
	UpdateSKMeshParts(rowName);
}

void ASTPlayerCharacter::ConvertRowNametoPartsName(FPlayerSKMeshesRowName RowName)
{
	SKMeshPartsName.Name_Head = RowName.Name_Head;
	SKMeshPartsName.Name_Hood = RowName.Name_Hood;
	SKMeshPartsName.Name_LongHair = RowName.Name_LongHair;
	SKMeshPartsName.Name_Glasses = RowName.Name_Glasses;
	SKMeshPartsName.Name_UpperBody = RowName.Name_UpperBody;
	SKMeshPartsName.Name_Backpack = RowName.Name_Backpack;
	SKMeshPartsName.Name_Hand_L = RowName.Name_Hand_L;
	SKMeshPartsName.Name_Hand_R = RowName.Name_Hand_R;
	SKMeshPartsName.Name_BottomBody = RowName.Name_BottomBody;
	SKMeshPartsName.Name_Feet = RowName.Name_Feet;
}

void ASTPlayerCharacter::ConvertPartsNametoRowName(FPlayerSKMeshesRowName& RowName)
{
	RowName.Name_Head = SKMeshPartsName.Name_Head;
	RowName.Name_Hood = SKMeshPartsName.Name_Hood;
	RowName.Name_LongHair = SKMeshPartsName.Name_LongHair;
	RowName.Name_Glasses = SKMeshPartsName.Name_Glasses;
	RowName.Name_UpperBody = SKMeshPartsName.Name_UpperBody;
	RowName.Name_Backpack = SKMeshPartsName.Name_Backpack;
	RowName.Name_Hand_L = SKMeshPartsName.Name_Hand_L;
	RowName.Name_Hand_R = SKMeshPartsName.Name_Hand_R;
	RowName.Name_BottomBody = SKMeshPartsName.Name_BottomBody;
	RowName.Name_Feet = SKMeshPartsName.Name_Feet;
}

void ASTPlayerCharacter::OnAttackSuccess_Server_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AActor* DamagedActor)
{
	OnAttackSuccess_Client();

	USTEnemyStatusComponent* EnemyStatusComp = DamagedActor->GetComponentByClass<USTEnemyStatusComponent>();
	if (!EnemyStatusComp) return;

	//// Item (Severance Matrix)
	//const FInvSlotStruct& ItemInfo_SM = InventoryComponent->GetItem(7);
	//if (ItemInfo_SM.ItemID != 0 && ItemInfo_SM.Quantity > 0)
	//{
	//	if ((DamageAmount / EnemyStatusComp->GetMaxHP()) > 0.5f)
	//	{
	//		DamagedActor->TakeDamage(5.f * ItemInfo_SM.Quantity, DamageEvent, GetController(), this);
	//	}
	//}

	//// Item (Combo Amplifier)
	//const FInvSlotStruct& ItemInfo_CA = InventoryComponent->GetItem(8);
	//if (ItemInfo_CA.ItemID != 0 && ItemInfo_CA.Quantity > 0)
	//{
	//	if(ItemInfo_CA.ItemObject)
	//	{
	//		ItemInfo_CA.ItemObject->OnAttackSuccess(DamageAmount, DamageEvent, DamagedActor);
	//	}
	//}

	const TArray<FInvSlotStruct>& Inventory = InventoryComponent->GetInventory();
	for (auto& item : Inventory)
	{
		if (item.ItemObject)
		{
			item.ItemObject->OnAttackSuccess(DamageAmount, DamageEvent, DamagedActor);
		}
	}

}

void ASTPlayerCharacter::OnAttackSuccess_Client_Implementation()
{
}