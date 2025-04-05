// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableCharacter.h"
#include "Actors/Interact/InteractableCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AInteractableCharacter::AInteractableCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AInteractableCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractableCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AInteractableCharacter, bIsInteractable);
}

void AInteractableCharacter::Interact(UInteractComponent* InteractingComponent)
{
}

void AInteractableCharacter::Interact_Finish(UInteractComponent* InteractingComponent)
{
}

void AInteractableCharacter::ShowInteractiveUI(UInteractComponent* InteractingComponent)
{
	if (!bIsInteractable)
		return;

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
