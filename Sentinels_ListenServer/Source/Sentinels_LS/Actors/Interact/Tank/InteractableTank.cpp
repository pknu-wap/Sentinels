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

void AInteractableTank::BeginPlay()
{
	Super::BeginPlay();

	TankAnimInst = Cast<UTankAnimInstance>(GetMesh()->GetAnimInstance());

	UCameraComponent* Camera = GetComponentByClass<UCameraComponent>();
	if (Camera)
	{
		CameraLocation_Normal = Camera->GetRelativeLocation();
	}
}

void AInteractableTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AInteractableTank::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
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
				EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AInteractableTank::Move);
				EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AInteractableTank::Look);
				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AInteractableTank::Fire_Pressed);
				EnhancedInputComponent->BindAction(ConvertModeAction, ETriggerEvent::Started, this, &AInteractableTank::ConvertMode_Pressed);
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
		AddMovementInput(GetActorRightVector(), MovementVector.X);
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
	Fire_Pressed_Server();
}

void AInteractableTank::Fire_Pressed_Server_Implementation()
{
	FVector SpawnLocation = GetMesh()->GetSocketLocation(FName("gun_jntSocket"));
	FRotator SpawnRotation = GetMesh()->GetSocketRotation(FName("turret_jnt"));

	AProjectileBase* Rocket = GetWorld()->SpawnActor<AProjectileBase>(RocketSubClass, SpawnLocation, SpawnRotation);
	if (Rocket)
	{
		Rocket->FireInDirection(SpawnRotation.Vector());
		// Rocket->FireInDirection(GetActorForwardVector());
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

void AInteractableTank::Interact(UInteractComponent* InteractingComponent)
{
	if (!bIsInteractable) return;

	APlayerController* PC = Cast<APlayerController>(InteractingComponent->GetOwner());
	if (PC)
	{
		PC->Possess(this);

		if (TankAnimInst)
		{
			TankAnimInst->SetController(PC);
		}

		bIsInteractable = false;
	}
}

void AInteractableTank::Interact_Finish(UInteractComponent* InteractingComponent)
{
}
