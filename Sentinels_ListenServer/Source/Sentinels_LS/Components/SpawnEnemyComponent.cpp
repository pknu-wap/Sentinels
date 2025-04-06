// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SpawnEnemyComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/Enemy/STEnemyBase_AIController.h"
#include "Character/Enemy/STEnemyBase.h"


// Sets default values for this component's properties
USpawnEnemyComponent::USpawnEnemyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void USpawnEnemyComponent::StartSpawnEnemy()
{
	if(!GetWorld()->GetTimerManager().TimerExists(handle))
		GetWorld()->GetTimerManager().SetTimer(handle, this, &USpawnEnemyComponent::SpawnEnemy, SpawnPeriod, true);
}

void USpawnEnemyComponent::StopSpawnEnemy()
{
	GetWorld()->GetTimerManager().ClearTimer(handle);
}

void USpawnEnemyComponent::SpawnEnemy()
{
	if (SpawnPawnClasses.Num() == 0)
	{
		UE_LOG(LogTemp, Display, TEXT("USpawnEnemyComponent : Pawn Classes are not setted!"));
		return;
	}

	if (GetOwner()->HasAuthority())
	{
		UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
		if (!NavSystem) return;

		int rand;
		for (int i = 0; i < SpawnRate; i++)
		{
			if (CurrentSpawned >= MaxSpawnCount)
				break;

			FNavLocation SpawnNavLocation;
			NavSystem->GetRandomReachablePointInRadius(GetOwner()->GetActorLocation(), SpawnableRadius_Outer, SpawnNavLocation);

			int maxLoopIdx = 50; int currentLoopIdx = 0;
			while (FVector::Dist2D(SpawnNavLocation.Location, GetOwner()->GetActorLocation()) < SpawnableRadius_Inner && currentLoopIdx <= maxLoopIdx)
			{
				NavSystem->GetRandomReachablePointInRadius(GetOwner()->GetActorLocation(), SpawnableRadius_Outer, SpawnNavLocation);
				currentLoopIdx++;
			}

			if(currentLoopIdx >= 50)
				UE_LOG(LogTemp, Warning, TEXT("USpawnEnemyComponent : Can't get Point between Inner and Outer Circle!"));

			rand = UKismetMathLibrary::RandomIntegerInRange(0, SpawnPawnClasses.Num() - 1);
			if (SpawnPawnClasses[rand])
			{
				APawn* pawn = UAIBlueprintHelperLibrary::SpawnAIFromClass(this, SpawnPawnClasses[rand], nullptr,
					SpawnNavLocation.Location, GetOwner()->GetActorRotation());

				// Check It is enemy
				if (ASTEnemyBase* Enemy = Cast<ASTEnemyBase>(pawn))
				{
					ASTEnemyBase_AIController* controller = Cast<ASTEnemyBase_AIController>(Enemy->GetController());
					if (controller && bShouldTargetOwner)
					{
						controller->SetTarget(GetOwner());
					}

					// Bind Function on Enemy Died!
					Enemy->Delegate_OnEnemyDied.AddDynamic(this, &USpawnEnemyComponent::OnEnemyDied);
					CurrentSpawned++;
					SpawnedEnemys.Push(pawn);
				}
			}
		}
	}
}

void USpawnEnemyComponent::OnEnemyDied(AActor* DiedEnemy)
{
	// Called when enemy died
	if (DiedEnemy)
	{
		CurrentSpawned--;
		SpawnedEnemys.Remove(DiedEnemy);
	}
}


