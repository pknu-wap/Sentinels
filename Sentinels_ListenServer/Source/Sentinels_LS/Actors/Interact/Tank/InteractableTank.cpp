// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interact/Tank/InteractableTank.h"
#include "Components/InteractComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "TankAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actors/Projectile/ProjectileBase.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"

AInteractableTank::AInteractableTank()
{
	GetOffSceneComponent = CreateDefaultSubobject<USceneComponent>(FName("Get Off Position"));
	GetOffSceneComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AInteractableTank::BeginPlay()
{
	Super::BeginPlay();

	TankAnimInst = Cast<UTankAnimInstance>(GetMesh()->GetAnimInstance());

	UCameraComponent* Camera = GetComponentByClass<UCameraComponent>();
	if (Camera)
	{
		CameraLocation_Normal = Camera->GetRelativeLocation();
	}

	if (HasAuthority())
	{
		RemainedBullet = MaxBullet;
	}
}

void AInteractableTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AInteractableTank::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractableTank, RemainedBullet);
}

void AInteractableTank::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PossessedCallback_Multicast();
}

void AInteractableTank::PossessedCallback_Multicast_Implementation()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// Set Input
		if (PlayerController->IsLocalController())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				// Clear Input Mapping
				Subsystem->ClearAllMappings();

				// Set Tank Input Mapping
				Subsystem->AddMappingContext(MappingContext_Tank, 0);
			}

			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				EnhancedInputComponent->ClearAxisBindings();
				EnhancedInputComponent->ClearBindingsForObject(this);

				EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AInteractableTank::Move);
				EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AInteractableTank::Look);
				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AInteractableTank::Fire_Pressed);
				EnhancedInputComponent->BindAction(ConvertModeAction, ETriggerEvent::Started, this, &AInteractableTank::ConvertMode_Pressed);
				EnhancedInputComponent->BindAction(GetOffAction, ETriggerEvent::Started, this, &AInteractableTank::GetOffFromTank);

			}
		}
	}
}

void AInteractableTank::Move(const FInputActionValue& Value)
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
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		
		FRotator CurrentRotation = GetActorRotation();
		SetActorRotation(CurrentRotation + FRotator(0, MovementVector.X * 0.3, 0));
		// AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AInteractableTank::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X * 0.2);
		AddControllerPitchInput(LookAxisVector.Y * 0.2);
	}
}

void AInteractableTank::Fire_Pressed()
{
	if (RemainedBullet <= 0) return;

	Fire_Pressed_Server();
}

void AInteractableTank::Fire_Pressed_Server_Implementation()
{
	if (RemainedBullet <= 0) return;

	FVector SpawnLocation = GetMesh()->GetSocketLocation(FName("gun_jntSocket"));
	FRotator SpawnRotation = GetMesh()->GetSocketRotation(FName("turret_jnt"));

	AProjectileBase* Rocket = GetWorld()->SpawnActor<AProjectileBase>(RocketSubClass, SpawnLocation, SpawnRotation);
	if (Rocket)
	{
		Rocket->FireInDirection(SpawnRotation.Vector());
		RemainedBullet = FMath::Clamp(RemainedBullet - 1, 0, MaxBullet);
	}
}

void AInteractableTank::ConvertMode_Pressed()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (CurrentCameraMode == ETankCameraMode::ETC_Normal)
		{
			CurrentCameraMode = ETankCameraMode::ETC_Focus;

			if (UCameraComponent* Camera = GetComponentByClass<UCameraComponent>())
			{
				Camera->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("gun_jntSocket"));
				Camera->SetRelativeLocation(FVector::ZeroVector);
			}

			CrossHairWidget = CreateWidget<UUserWidget>(PlayerController, CrossHairWidgetClass);
			if (CrossHairWidget)
			{
				CrossHairWidget->AddToViewport();
			}
		}
		else
		{
			CurrentCameraMode = ETankCameraMode::ETC_Normal;

			if (UCameraComponent* Camera = GetComponentByClass<UCameraComponent>())
			{
				Camera->AttachToComponent(GetComponentByClass<USpringArmComponent>(), FAttachmentTransformRules::KeepRelativeTransform);
				Camera->SetRelativeLocation(CameraLocation_Normal);
			}

			if (CrossHairWidget)
			{
				CrossHairWidget->RemoveFromParent();
			}
		}
	}
}

void AInteractableTank::GetOffFromTank()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (CurrentCameraMode == ETankCameraMode::ETC_Focus)
		{
			CurrentCameraMode = ETankCameraMode::ETC_Normal;

			if (UCameraComponent* Camera = GetComponentByClass<UCameraComponent>())
			{
				Camera->AttachToComponent(GetComponentByClass<USpringArmComponent>(), FAttachmentTransformRules::KeepRelativeTransform);
				Camera->SetRelativeLocation(CameraLocation_Normal);
			}

			if (CrossHairWidget)
			{
				CrossHairWidget->RemoveFromParent();
			}
		}
	}

	GetOffFromTank_Server();
}

void AInteractableTank::GetOffFromTank_Server_Implementation()
{
	if (CachedPC && CachedPlayerPawn)
	{
		FVector SpawnLocation = GetActorLocation() + GetOffSceneComponent->GetRelativeLocation();
		CachedPlayerPawn->SetActorLocation(SpawnLocation);
		CachedPlayerPawn->SetActorHiddenInGame(false);

		CachedPC->Possess(CachedPlayerPawn);

		bIsInteractable = true;
	}
}

void AInteractableTank::Interact_Implementation(UInteractComponent* InteractingComponent)
{
	if (!bIsInteractable) return;

	if (RemainedBullet <= 0) return;

	CachedPC = Cast<APlayerController>(InteractingComponent->GetOwner());
	if (CachedPC)
	{
		CachedPlayerPawn = CachedPC->GetPawn();
		if (CachedPlayerPawn)
		{
			CachedPlayerPawn->SetActorHiddenInGame(true);
		}

		CachedPC->Possess(this);

		if (TankAnimInst)
		{
			TankAnimInst->SetController(CachedPC);
		}

		bIsInteractable = false;
	}
}

void AInteractableTank::Interact_Finish_Implementation(UInteractComponent* InteractingComponent)
{
}
