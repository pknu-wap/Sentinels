// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractComponent.h"
#include "Components/InventoryComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/InteractiveInterface.h"
#include "Net/UnrealNetwork.h"
#include "Character/STCharacterBase.h"
#include "STGameplayTags.h"
#include "Player/STPlayerAnimInstance.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	AController* controller = Cast<AController>(GetOwner());
	if (controller && controller->IsLocalPlayerController())
	{
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, this, &UInteractComponent::FindInteractiveActor, 0.05f, true);
	}

	ASTCharacterBase* character = Cast<ASTCharacterBase>(controller->GetPawn());
	if (character)
	{
		USTPlayerAnimInstance* animInst = Cast<USTPlayerAnimInstance>(character->GetMesh()->GetAnimInstance());
		if (animInst)
		{
			animInst->Delegate_ThrowLiftingActor.AddUObject(this, &UInteractComponent::ThrowLiftingActor);
		}
	}
}

void UInteractComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInteractComponent, LiftingActor);
}

void UInteractComponent::Interact_Server_Implementation()
{
	AController* controller = Cast<AController>(GetOwner());
	if (!controller) return;

	ASTCharacterBase* character = Cast< ASTCharacterBase>(controller->GetPawn());
	if (!character) return;

	// Check Character is Lifting Anything
	if (character->HasTag(FSTGameplayTags::Get().Character_Player_State_Lifting))
	{
		// Play Throw Montage
		PlayThrowMontage_Server();
	}

	// Normal Interact
	// Find Interactable Actor & Try Interact
	else
	{
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(character);

		FVector start = character->GetActorLocation();
		FRotator ControllerRot = controller->GetControlRotation();
		FVector ControllerForwardVec = UKismetMathLibrary::GetForwardVector(ControllerRot);
		// FVector end = start + Interact_Range * ControllerForwardVec;
		FVector end = start + Interact_Range * character->GetActorForwardVector();

		ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);

		FHitResult hitResult;
		UKismetSystemLibrary::SphereTraceSingle(character, start, end, Interact_Radius,
			TraceType, false, IgnoreActors,
			EDrawDebugTrace::None, hitResult, true);

		if (hitResult.bBlockingHit)
		{
			InteractiveObject = Cast<IInteractiveInterface>(hitResult.GetActor());
			{
				if (InteractiveObject && IInteractiveInterface::Execute_IsInteractable(hitResult.GetActor()))
				{
					IInteractiveInterface::Execute_Interact(hitResult.GetActor(), this);
				}
			}
		}
	}
	
}

void UInteractComponent::Interact_Finish_Server_Implementation()
{
	if (InteractiveObject)
	{
		UObject* object = Cast<UObject>(InteractiveObject);
		if(object)
			IInteractiveInterface::Execute_Interact_Finish(object, this);
	}
}

void UInteractComponent::AttachLiftingActor_Server_Implementation(AActor* InActor, FName SocketName, FVector RelativeLocation, FRotator RelativeRotation)
{
	if (InActor)
	{
		APlayerController* owningController = Cast<APlayerController>(GetOwner());
		if (!owningController) return;

		ASTCharacterBase* character = Cast<ASTCharacterBase>(owningController->GetPawn());
		if (!character) return;

		// Set GameplayTag For Animation
		character->AddTag(FSTGameplayTags::Get().Character_Player_State_Lifting);

		// Set Lifting Actor & Set Relative Location and Rotation
		LiftingActor = InActor;
		InActor->SetActorLocation(FVector::ZeroVector);

		InActor->AttachToComponent(character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketName);
		InActor->SetActorRelativeLocation(RelativeLocation);
		InActor->SetActorRelativeRotation(RelativeRotation);

		// Disable Collision on Attached Actor
		if (UStaticMeshComponent* staticMesh = InActor->GetComponentByClass<UStaticMeshComponent>())
		{
			staticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void UInteractComponent::PlayThrowMontage_Server_Implementation()
{
	APlayerController* owningController = Cast<APlayerController>(GetOwner());
	if (!owningController) return;

	ASTCharacterBase* character = Cast<ASTCharacterBase>(owningController->GetPawn());
	if (!character) return;

	if (LiftingActor)
	{
		// Remove Tag For Animation
		character->RemoveTag(FSTGameplayTags::Get().Character_Player_State_Lifting);

		// Play Montage 
		PlayThrowMontage_Multicast();

		// Add Force To Lifting Actor


	}
}

void UInteractComponent::PlayThrowMontage_Multicast_Implementation()
{
	APlayerController* owningController = Cast<APlayerController>(GetOwner());
	if (!owningController) return;

	ASTCharacterBase* character = Cast<ASTCharacterBase>(owningController->GetPawn());
	if (!character) return;

	character->PlayAnimMontage(Montage_Throwing);
}

void UInteractComponent::ThrowLiftingActor()
{
	if (LiftingActor)
	{
		APlayerController* owningController = Cast<APlayerController>(GetOwner());
		if (!owningController) return;

		ASTCharacterBase* character = Cast<ASTCharacterBase>(owningController->GetPawn());
		if (!character || !character->HasAuthority()) return;

		LiftingActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		UStaticMeshComponent* staticMesh = LiftingActor->GetComponentByClass<UStaticMeshComponent>();
		if (staticMesh)
		{
			staticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			staticMesh->SetSimulatePhysics(true);
			staticMesh->AddImpulseAtLocation(character->GetActorForwardVector() * 100000, LiftingActor->GetActorLocation());
		}
	}

	// Remove Cached Lifting Actor
	LiftingActor = nullptr;
}

void UInteractComponent::StartInteractHold_Client_Implementation(float InHoldingTime)
{
	bIsHolding = true;
	Delegate_StartInteractHold.Broadcast(InHoldingTime);
}

void UInteractComponent::FinishInteractHold_Client_Implementation()
{
	bIsHolding = false;
	Delegate_FinishInteractHold.Broadcast();
}

void UInteractComponent::FindInteractiveActor()
{
	AController* controller = Cast<AController>(GetOwner());
	if (!controller) return;

	APawn* pawn = controller->GetPawn();
	if (!pawn) return;

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(pawn);

	FVector start = pawn->GetActorLocation();
	FRotator ControllerRot = controller->GetControlRotation();
	FVector ControllerForwardVec = UKismetMathLibrary::GetForwardVector(ControllerRot);
	// FVector end = start + Interact_Range * ControllerForwardVec;
	FVector end = start + Interact_Range * pawn->GetActorForwardVector();

	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);

	FHitResult hitResult;
	UKismetSystemLibrary::SphereTraceSingle(pawn, start, end, Interact_Radius,
		TraceType, false, IgnoreActors,
		EDrawDebugTrace::None, hitResult, true);

	if (hitResult.bBlockingHit)
	{
		InteractiveObject = Cast<IInteractiveInterface>(hitResult.GetActor());

		// Show Interactable UI 
		if (InteractiveObject)
		{
			IInteractiveInterface::Execute_ShowInteractiveUI(hitResult.GetActor(), this);
		}
	}
	else
	{
		if (InteractiveObject)
		{
			UObject* object = Cast<UObject>(InteractiveObject);
			if (object)
				IInteractiveInterface::Execute_HideInteractiveUI(object, this);
		}

		if (bIsHolding)
		{
			/*
				Should Cancel Interact In Server !  ! !
			*/
			Interact_Finish_Server();
			// InteractiveObject->Interact_Finish(this);
			FinishInteractHold_Client();
		}

		InteractiveObject = nullptr;
	}
}
