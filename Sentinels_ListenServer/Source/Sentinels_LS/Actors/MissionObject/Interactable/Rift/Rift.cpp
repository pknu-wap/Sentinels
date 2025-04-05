// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MissionObject/Interactable/Rift/Rift.h"
#include "Components/InteractComponent.h"

void ARift::Interact(UInteractComponent* InteractingComponent)
{
	if (HasAuthority())
	{
		// Caching Interact Component
		InteractedComponent = InteractingComponent;

		// Start Rescue
		GetWorldTimerManager().SetTimer(Handle_Hold, this, &ARift::RepairSuccessed, InteractionHoldTime, false);

		// Show Interacting Widget
		if (InteractedComponent)
		{
			InteractedComponent->StartInteractHold_Client(InteractionHoldTime);
		}
	}
}

void ARift::Interact_Finish(UInteractComponent* InteractingComponent)
{
	if (HasAuthority())
	{
		// Clear Timer
		GetWorldTimerManager().ClearTimer(Handle_Hold);

		// Hide Interacting Widget
		if (InteractedComponent)
		{
			InteractedComponent->FinishInteractHold_Client();
		}
	}
}

void ARift::RepairSuccessed()
{
	Delegate_MissionConditionUpdate.Broadcast(ObjectID, true);

	// Hide Interacting Widget On Client
	if (InteractedComponent)
	{
		InteractedComponent->FinishInteractHold_Client();
	}
}
