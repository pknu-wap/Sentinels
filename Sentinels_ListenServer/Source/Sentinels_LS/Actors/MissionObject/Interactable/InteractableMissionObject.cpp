// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MissionObject/Interactable/InteractableMissionObject.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AInteractableMissionObject::Interact()
{
}

void AInteractableMissionObject::ShowInteractiveUI()
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

void AInteractableMissionObject::HideInteractiveUI()
{
	if (InteractWidget_ForDebug)
	{
		InteractWidget_ForDebug->RemoveFromParent();
	}
}
