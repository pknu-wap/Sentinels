// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interact/InteractableActor.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

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

void AInteractableActor::Interact()
{
}

void AInteractableActor::ShowInteractiveUI()
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

void AInteractableActor::HideInteractiveUI()
{
	if (InteractWidget_ForDebug)
	{
		InteractWidget_ForDebug->RemoveFromParent();
	}
}
