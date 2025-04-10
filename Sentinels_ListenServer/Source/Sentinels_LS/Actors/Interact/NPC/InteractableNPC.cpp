// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interact/NPC/InteractableNPC.h"
#include "Components/InteractComponent.h"
#include "GameFramework/PlayerController.h"
#include "Character/STCharacterBase.h"
#include "STGameplayTags.h"

void AInteractableNPC::Interact(UInteractComponent* InteractingComponent)
{
	if (HasAuthority())
	{

	}
}

void AInteractableNPC::Interact_Finish(UInteractComponent* InteractingComponent)
{
	if (HasAuthority())
	{
		
	}
}
