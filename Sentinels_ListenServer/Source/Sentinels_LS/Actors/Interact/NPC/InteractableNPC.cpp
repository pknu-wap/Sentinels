// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interact/NPC/InteractableNPC.h"
#include "Components/InteractComponent.h"
#include "GameFramework/PlayerController.h"
#include "Character/STCharacterBase.h"
#include "STGameplayTags.h"

void AInteractableNPC::Interact_Implementation(UInteractComponent* InteractingComponent)
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

			APlayerController* PC = Cast<APlayerController>(InteractedComponent->GetOwner());
			if (PC)
			{
				ASTCharacterBase* Character = Cast<ASTCharacterBase>(PC->GetPawn());
				if (Character)
				{
					Character->AddTag(FSTGameplayTags::Get().Character_Player_State_RescueHostage);
				}
			}
		}
	}
}

void AInteractableNPC::Interact_Finish_Implementation(UInteractComponent* InteractingComponent)
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

			APlayerController* PC = Cast<APlayerController>(InteractedComponent->GetOwner());
			if (PC)
			{
				ASTCharacterBase* Character = Cast<ASTCharacterBase>(PC->GetPawn());
				if (Character)
				{
					Character->AddTag(FSTGameplayTags::Get().Character_Player_State_RescueHostage);
				}
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

		APlayerController* PC = Cast<APlayerController>(InteractedComponent->GetOwner());
		if (PC)
		{
			ASTCharacterBase* Character = Cast<ASTCharacterBase>(PC->GetPawn());
			if (Character)
			{
				Character->AddTag(FSTGameplayTags::Get().Character_Player_State_RescueHostage);
			}
		}
	}
}
