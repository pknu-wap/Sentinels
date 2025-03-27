// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MissionObject/NonInteractable/DominationPoint/DominationPoint.h"
#include "Components/SphereComponent.h"
#include "Components/SpawnEnemyComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/STPlayerCharacter.h"


ADominationPoint::ADominationPoint()
{
	TriggerSphereComponent = CreateDefaultSubobject<USphereComponent>(FName("SphereComp"));
	TriggerSphereComponent->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);

	SpawnEnemyComp = CreateDefaultSubobject<USpawnEnemyComponent>(FName("SpawnComp"));
}

void ADominationPoint::BeginPlay()
{
	if (HasAuthority())
	{
		TriggerSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADominationPoint::TriggerSphereBeginOverlap);
	}
}

void ADominationPoint::TriggerSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() && OtherActor && OtherActor->IsA<ASTPlayerCharacter>())
	{
		SpawnEnemyComp->StartSpawnEnemy();
	}
}
