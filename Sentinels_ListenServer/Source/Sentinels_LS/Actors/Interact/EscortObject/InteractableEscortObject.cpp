// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interact/EscortObject/InteractableEscortObject.h"
#include "Actors/SplineRoute/SplineRouteActor.h"
#include "Components/SplineComponent.h"
#include "System/STGameState.h"
#include "System/Mission/STMissionBase.h"
#include "STGameplayTags.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/Enemy/STEnemyBase_AIController.h"
#include "Components/SpawnEnemyComponent.h"

AInteractableEscortObject::AInteractableEscortObject()
{
	bReplicates = true;
	SetReplicateMovement(true);
	
	PrimaryActorTick.bCanEverTick = true;

	SpawnEnemyComp = CreateDefaultSubobject<USpawnEnemyComponent>(FName("SpawnComp"));
}

void AInteractableEscortObject::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
}

void AInteractableEscortObject::Tick(float DeltaSeconds)
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
				// StopMove();
			}
		}
	}
}

void AInteractableEscortObject::StartMoveAlongSpline()
{
	if (HasAuthority())
	{
		bShouldMove = true;
		SetActorTickEnabled(true);

		// Enable Spawn Enemy while Escort Mission
		SpawnEnemyComp->StartSpawnEnemy();
	}
}

void AInteractableEscortObject::StopMove()
{
	if (HasAuthority())
	{
		bShouldMove = false;
		SetActorTickEnabled(false);

		// Update Mission Info
		ASTGameState* GameState = Cast<ASTGameState>(UGameplayStatics::GetGameState(this));
		if (GameState)
		{
			USTMissionBase* mission = GameState->GetMission(FSTGameplayTags::Get().Mission_EscortObject);
			if (mission)
			{
				mission->UpdateEscortObjectInfo_Server(EscortObjectIdx, true);
			}
		}

		// Disable Spawn Enemy while Escort Mission
		SpawnEnemyComp->StopSpawnEnemy();
	}
}

void AInteractableEscortObject::Interact()
{
	if(HasAuthority())
		StartMoveAlongSpline();
}
