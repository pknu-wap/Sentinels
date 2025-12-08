// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MissionObject/MissionObject.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMissionObject::AMissionObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("RootComponent"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMesh"));
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	bReplicates = true;
}

void AMissionObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMissionObject, bIsSuccessed);
}

void AMissionObject::OnRep_bIsSuccessed()
{
	if (bIsSuccessed)
		Delegate_MissionConditionUpdate.Broadcast(ObjectID, true);
}


