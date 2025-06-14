// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MissionObject/Interactable/EscortObject/EscortObject.h"
#include "Actors/SplineRoute/SplineRouteActor.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SpawnEnemyComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/Enemy/STEnemyBase_AIController.h"
#include "Player/STPlayerCharacter.h"

AEscortObject::AEscortObject()
{
	bReplicates = true;
	SetReplicateMovement(true);

	PrimaryActorTick.bCanEverTick = true;

	SpawnEnemyComp = CreateDefaultSubobject<USpawnEnemyComponent>(FName("SpawnComp"));

	BoxComp = CreateDefaultSubobject<UBoxComponent>(FName("BoxComponent"));
	BoxComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AEscortObject::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	if (BoxComp)
	{
		BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AEscortObject::BoxBeginOverlapped);
		BoxComp->OnComponentEndOverlap.AddDynamic(this, &AEscortObject::BoxEndOverlapped);
	}
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

				// Update Mission Info
				Delegate_MissionConditionUpdate.Broadcast(ObjectID, true);

				// Disable Spawn Enemy while Escort Mission
				SpawnEnemyComp->StopSpawnEnemy();
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
	}
}

void AEscortObject::Interact_Implementation(UInteractComponent* InteractingComponent)
{
	if (HasAuthority())
	{
		StartMoveAlongSpline();
		bIsInteractable = false;
	}
}

void AEscortObject::BoxBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASTPlayerCharacter* player = Cast<ASTPlayerCharacter>(OtherActor);
	if (player)
	{
		StartMoveAlongSpline();
	}
}

void AEscortObject::BoxEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASTPlayerCharacter* player = Cast<ASTPlayerCharacter>(OtherActor);
	if (player)
	{
		StopMove();
	}
}
