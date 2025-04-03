// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MissionObject/MissionObject.h"

// Sets default values
AMissionObject::AMissionObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMesh"));
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}


