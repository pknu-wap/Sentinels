// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetComponent/STWC_LocalPlayerFacing.h"

USTWC_LocalPlayerFacing::USTWC_LocalPlayerFacing()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void USTWC_LocalPlayerFacing::OnRegister()
{
	Super::OnRegister();

	SetWidgetSpace(EWidgetSpace::World);
}

void USTWC_LocalPlayerFacing::BeginPlay()
{
	Super::BeginPlay();

	FVector currentLoc = GetComponentLocation();

	currentLoc.Z += Height;
	SetWorldLocation(currentLoc);
}

void USTWC_LocalPlayerFacing::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* owner = GetOwner();
	APlayerController* pc = GetWorld()->GetFirstPlayerController();

	if (!owner || !pc)
		return;

	APlayerCameraManager* cm = pc->PlayerCameraManager;
	if (!cm)
		return;

	FVector toCamera = cm->GetCameraLocation() - GetComponentLocation();
	toCamera.Z = 0.f;

	if (!toCamera.IsNearlyZero())
	{
		FRotator lookAtRotation = toCamera.Rotation();
		SetWorldRotation(lookAtRotation);
	}
}
