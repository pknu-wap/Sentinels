// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableCharacter.h"
#include "Actors/Interact/InteractableCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AInteractableCharacter::AInteractableCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AInteractableCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractableCharacter::Interact(UInteractComponent* InteractingComponent)
{
}

void AInteractableCharacter::Interact_Finish(UInteractComponent* InteractingComponent)
{
}

void AInteractableCharacter::ShowInteractiveUI(UInteractComponent* InteractingComponent)
{
	if (InteractWidget_ForDebug)
	{
		InteractWidget_ForDebug->RemoveFromParent();
	}

	if (InteractWidgetClass_ForDebug)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		if (PC)
		{
			InteractWidget_ForDebug = CreateWidget<UUserWidget>(PC, InteractWidgetClass_ForDebug);
			if (InteractWidget_ForDebug)
			{
				InteractWidget_ForDebug->AddToViewport();
			}
		}
	}
}

void AInteractableCharacter::HideInteractiveUI(UInteractComponent* InteractingComponent)
{
	if (InteractWidget_ForDebug)
	{
		InteractWidget_ForDebug->RemoveFromParent();
	}
}
