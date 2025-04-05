// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interact/InteractableActor.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AInteractableActor, bIsInteractable);
}

void AInteractableActor::Interact(UInteractComponent* InteractingComponent)
{
}

void AInteractableActor::Interact_Finish(UInteractComponent* InteractingComponent)
{
}

void AInteractableActor::ShowInteractiveUI(UInteractComponent* InteractingComponent)
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

void AInteractableActor::HideInteractiveUI(UInteractComponent* InteractingComponent)
{
	if (InteractWidget_ForDebug)
	{
		InteractWidget_ForDebug->RemoveFromParent();
	}
}
