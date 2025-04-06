// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractComponent.h"
#include "Components/InventoryComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/InteractiveInterface.h"

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
}

void UInteractComponent::Interact_Server_Implementation()
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
		EDrawDebugTrace::ForOneFrame, hitResult, true);

	if (hitResult.bBlockingHit)
	{
		InteractiveObject = Cast<IInteractiveInterface>(hitResult.GetActor());
		{
			if (InteractiveObject && InteractiveObject->IsInteractable())
			{
				InteractiveObject->Interact(this);
			}
		}
	}
}

void UInteractComponent::Interact_Finish_Server_Implementation()
{
	if (InteractiveObject)
	{
		InteractiveObject->Interact_Finish(this);
	}
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
		EDrawDebugTrace::ForOneFrame, hitResult, true);

	if (hitResult.bBlockingHit)
	{
		InteractiveObject = Cast<IInteractiveInterface>(hitResult.GetActor());

		// Show Interactable UI 
		if (InteractiveObject)
		{
			InteractiveObject->ShowInteractiveUI(this);
		}
	}
	else
	{
		if (InteractiveObject)
		{
			InteractiveObject->HideInteractiveUI(this);
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
