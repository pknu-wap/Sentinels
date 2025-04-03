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
	Super::BeginPlay();

	if (HasAuthority())
	{
		TriggerSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADominationPoint::TriggerSphereBeginOverlap);
	}
}

void ADominationPoint::Tick(float fDeltaTime)
{
	Super::Tick(fDeltaTime);

	if (HasAuthority() && OverlappedPlayer > 0)
	{
		currentDominationTime = FMath::Clamp(currentDominationTime + fDeltaTime, 0, RequireTimeForDomination);
		if (currentDominationTime >= RequireTimeForDomination)
		{
			Delegate_MissionConditionUpdate.Broadcast(ObjectID, true);
		}
	}
}

void ADominationPoint::TriggerSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() && OtherActor && OtherActor->IsA<ASTPlayerCharacter>())
	{
		OverlappedPlayer++;
		SpawnEnemyComp->StartSpawnEnemy();
	}
}

void ADominationPoint::TriggerSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasAuthority() && OtherActor && OtherActor->IsA<ASTPlayerCharacter>())
	{
		OverlappedPlayer--;

		if (OverlappedPlayer == 0)
		{
			SpawnEnemyComp->StopSpawnEnemy();

			currentDominationTime = 0.f;
		}
	}
}
