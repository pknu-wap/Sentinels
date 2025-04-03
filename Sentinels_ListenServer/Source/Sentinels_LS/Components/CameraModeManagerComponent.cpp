// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CameraModeManagerComponent.h"
#include "Player/Camera/CameraMode.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values for this component's properties
UCameraModeManagerComponent::UCameraModeManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCameraModeManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	SpringArm = GetOwner()->FindComponentByClass<USpringArmComponent>();

	if (DefaultCameraMode)
	{
		SetCameraMode(DefaultCameraMode);
	}
}


// Called every frame
void UCameraModeManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentMode)
	{
		CurrentMode->Tick(DeltaTime);
	}
}

void UCameraModeManagerComponent::SetCameraMode(TSubclassOf<UCameraMode> NewModeClass)
{
	if (CurrentMode)
	{
		CurrentMode->OnExit();
	}

	CurrentMode = NewObject<UCameraMode>(this, NewModeClass);
	if (CurrentMode)
	{
		CurrentMode->OnEnter(this);
	}
}

