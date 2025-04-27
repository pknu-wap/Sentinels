// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interact/InteractablePawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AInteractablePawn::AInteractablePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMesh"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AInteractablePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractablePawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AInteractablePawn, bIsInteractable);
}

// Called to bind functionality to input
void AInteractablePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AInteractablePawn::Interact(UInteractComponent* InteractingComponent)
{

}

void AInteractablePawn::Interact_Finish(UInteractComponent* InteractingComponent)
{

}

void AInteractablePawn::ShowInteractiveUI(UInteractComponent* InteractingComponent)
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

void AInteractablePawn::HideInteractiveUI(UInteractComponent* InteractingComponent)
{
	if (InteractWidget_ForDebug)
	{
		InteractWidget_ForDebug->RemoveFromParent();
	}
}

