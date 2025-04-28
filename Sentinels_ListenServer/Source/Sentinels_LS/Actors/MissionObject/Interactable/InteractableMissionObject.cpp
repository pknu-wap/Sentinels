// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MissionObject/Interactable/InteractableMissionObject.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AInteractableMissionObject::AInteractableMissionObject()
{
	
}

void AInteractableMissionObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AInteractableMissionObject, bIsInteractable)
}

void AInteractableMissionObject::Interact_Implementation(UInteractComponent* InteractingComponent)
{
}

void AInteractableMissionObject::Interact_Finish_Implementation(UInteractComponent* InteractingComponent)
{
}

void AInteractableMissionObject::ShowInteractiveUI_Implementation(UInteractComponent* InteractingComponent)
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

void AInteractableMissionObject::HideInteractiveUI_Implementation(UInteractComponent* InteractingComponent)
{
	if (InteractWidget_ForDebug)
	{
		InteractWidget_ForDebug->RemoveFromParent();
	}
}
