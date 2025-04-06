// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interact/NPC/InteractableNPC.h"
#include "Components/InteractComponent.h"
#include "Character/STCharacterBase.h"
#include "STGameplayTags.h"

void AInteractableNPC::Interact(UInteractComponent* InteractingComponent)
{
	if (HasAuthority())
	{
		// Caching Interact Component
		InteractedComponent = InteractingComponent;

		// Start Rescue
		GetWorldTimerManager().SetTimer(Handle_Hold, this, &AInteractableNPC::RescueSuccessed, InteractionHoldTime, false);

		// 1. Show Interacting Widget
		// 2. Add tag to Player
		if (InteractedComponent)
		{
			InteractedComponent->StartInteractHold_Client(InteractionHoldTime);

			ASTCharacterBase* character = Cast<ASTCharacterBase>(InteractedComponent->GetOwner());
			if (character)
			{
				character->AddTag(FSTGameplayTags::Get().Character_Player_State_RescueHostage);
			}
		}
	}
}

void AInteractableNPC::Interact_Finish(UInteractComponent* InteractingComponent)
{
	if (HasAuthority())
	{
		// Clear Timer
		GetWorldTimerManager().ClearTimer(Handle_Hold);

		// 1. Hide Interacting Widget
		// 2, Remove tag from Player
		if (InteractedComponent)
		{
			InteractedComponent->FinishInteractHold_Client();

			ASTCharacterBase* character = Cast<ASTCharacterBase>(InteractedComponent->GetOwner());
			if (character)
			{
				character->RemoveTag(FSTGameplayTags::Get().Character_Player_State_RescueHostage);
			}
		}
	}
}

void AInteractableNPC::RescueSuccessed()
{
	bIsInteractable = false;

	Delegate_MissionConditionUpdate.Broadcast(NPCID, true);

	// 1. Hide Interacting Widget
	// 2, Remove tag from Player
	if (InteractedComponent)
	{
		InteractedComponent->FinishInteractHold_Client();

		ASTCharacterBase* character = Cast<ASTCharacterBase>(InteractedComponent->GetOwner());
		if (character)
		{
			character->RemoveTag(FSTGameplayTags::Get().Character_Player_State_RescueHostage);
		}
	}
}
