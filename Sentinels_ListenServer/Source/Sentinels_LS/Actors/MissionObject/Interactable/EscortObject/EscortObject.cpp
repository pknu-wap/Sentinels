// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MissionObject/Interactable/EscortObject/EscortObject.h"
#include "Actors/SplineRoute/SplineRouteActor.h"
#include "Components/SplineComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/Enemy/STEnemyBase_AIController.h"
#include "Components/SpawnEnemyComponent.h"

AEscortObject::AEscortObject()
{
	bReplicates = true;
	SetReplicateMovement(true);

	PrimaryActorTick.bCanEverTick = true;

	SpawnEnemyComp = CreateDefaultSubobject<USpawnEnemyComponent>(FName("SpawnComp"));
}

void AEscortObject::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
}

void AEscortObject::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority())
	{
		if (bShouldMove && SplineActor)
		{
			DistanceAlongSpline += MoveSpeed * DeltaSeconds;

			FVector NewLocation = SplineActor->GetSplineLocationAtDistance(DistanceAlongSpline);
			SetActorLocation(NewLocation);

			FRotator NewRotation = SplineActor->GetSplineRotationAtDistance(DistanceAlongSpline);
			SetActorRotation(NewRotation);

			if (DistanceAlongSpline >= SplineActor->GetSplineLength())
			{
				SpawnEnemyComp->StopSpawnEnemy();
				StopMove();
			}
		}
	}
}

void AEscortObject::StartMoveAlongSpline()
{
	if (HasAuthority())
	{
		bShouldMove = true;
		SetActorTickEnabled(true);

		// Enable Spawn Enemy while Escort Mission
		SpawnEnemyComp->StartSpawnEnemy();
	}
}

void AEscortObject::StopMove()
{
	if (HasAuthority())
	{
		bShouldMove = false;
		SetActorTickEnabled(false);

		// Update Mission Info
		Delegate_MissionConditionUpdate.Broadcast(ObjectID, true);
		
		// Disable Spawn Enemy while Escort Mission
		SpawnEnemyComp->StopSpawnEnemy();
	}
}

void AEscortObject::Interact(UInteractComponent* InteractingComponent)
{
	if (HasAuthority())
		StartMoveAlongSpline();
}
