// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/STPlayerController.h"
#include "EnhancedInputComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Components/SplineComponent.h"
#include "DrawDebugHelpers.h"

ASTPlayerController::ASTPlayerController()
{
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void ASTPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveClickAction, ETriggerEvent::Started, this, &ASTPlayerController::MoveClick_Started);
		EnhancedInputComponent->BindAction(MoveClickAction, ETriggerEvent::Triggered, this, &ASTPlayerController::MoveClick_Triggered);
		EnhancedInputComponent->BindAction(MoveClickAction, ETriggerEvent::Completed, this, &ASTPlayerController::MoveClick_Released);
		EnhancedInputComponent->BindAction(MoveClickAction, ETriggerEvent::Canceled, this, &ASTPlayerController::MoveClick_Released);
	}
}

void ASTPlayerController::MoveClick_Started()
{
	StopMovement();
	GetWorldTimerManager().ClearTimer(Handle_AutoRun);
}

void ASTPlayerController::MoveClick_Triggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void ASTPlayerController::MoveClick_Released()
{
	// If it was a short press
	APawn* ControlledPawn = GetPawn();
	if (FollowTime <= ShortPressThreshold)
	{
		if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetTargetLocation(), CachedDestination))
		{
			Spline->ClearSplinePoints();

			for (const FVector& point : NavPath->PathPoints)
			{
				Spline->AddSplinePoint(point, ESplineCoordinateSpace::World);
				DrawDebugSphere(GetWorld(), point, 10.f, 8, FColor::Green, false, 5.f);
			}

			GetWorldTimerManager().SetTimer(Handle_AutoRun, this, &ASTPlayerController::AutoRun, 0.015f, true);
		}
	}

	FollowTime = 0.f;
}

void ASTPlayerController::AutoRun()
{
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector NextLocation = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector NextDirection = Spline->FindDirectionClosestToWorldLocation(NextLocation, ESplineCoordinateSpace::World);

		ControlledPawn->AddMovementInput(NextDirection);

		const float DistanceDestination = (NextLocation - CachedDestination).Length();
		if (DistanceDestination <= AutoRunAcceptanceRadius)
		{
			StopMovement();
			GetWorldTimerManager().ClearTimer(Handle_AutoRun);
		}
	}
}
