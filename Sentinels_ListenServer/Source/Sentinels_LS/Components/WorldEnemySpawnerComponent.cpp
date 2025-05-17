// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WorldEnemySpawnerComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/Enemy/STEnemyBase_AIController.h"
#include "Character/Enemy/STEnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/ObjectPool/CharacterObjectPool.h"
#include "Components/BoxComponent.h"

// Sets default values for this component's properties
UWorldEnemySpawnerComponent::UWorldEnemySpawnerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	
}

void UWorldEnemySpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto& SpawnInfo : SpawnInfos)
	{
		SpawnInfo.ObjectPools.Reserve(SpawnInfo.SpawnPawnClasses.Num());

		for (int i = 0; i < SpawnInfo.SpawnPawnClasses.Num(); i++)
		{
			ACharacterObjectPool* ObjectPool =
				GetWorld()->SpawnActor<ACharacterObjectPool>(ACharacterObjectPool::StaticClass());

			if (ObjectPool)
			{
				ObjectPool->InitPool(SpawnInfo.SpawnPawnClasses[i], SpawnInfo.MaxSpawnCount);
				SpawnInfo.ObjectPools.Add(ObjectPool);
			}
		}
	}
}

void UWorldEnemySpawnerComponent::StartSpawnEnemy()
{
	if (!GetOwner()->HasAuthority())
		return;

	for (int i = 0; i < SpawnInfos.Num(); i++)
	{
		if (!GetWorld()->GetTimerManager().TimerExists(SpawnInfos[i].TimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(SpawnInfos[i].TimerHandle,
				[this, i]()
				{
					SpawnEnemy(i);
				}
			, SpawnInfos[i].SpawnPeriod, true);
		}
	}
}

void UWorldEnemySpawnerComponent::StopSpawnEnemy()
{
	for (int i = 0; i < SpawnInfos.Num(); i++)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnInfos[i].TimerHandle);
	}
}

void UWorldEnemySpawnerComponent::SpawnEnemy(int InfoIdx)
{
	if (!SpawnInfos.IsValidIndex(InfoIdx))
	{
		return;
	}

	FSpawnInfo& Info = SpawnInfos[InfoIdx];

	int rand; FNavLocation SpawnNavLocation;
	for (int i = 0; i < Info.SpawnRate; i++)
	{
		if (Info.CurrentSpawned >= Info.MaxSpawnCount)
			break;

		// Check Get NavLocation Success && Check Class is valid 
		rand = UKismetMathLibrary::RandomIntegerInRange(0, Info.SpawnPawnClasses.Num() - 1);
		if (!GetSpawnNavLocation(InfoIdx, SpawnNavLocation) || !Info.SpawnPawnClasses[rand])
			continue;

		// Spawn Enemy
		SpawnNavLocation.Location.Z += 75.f;

		if (Info.ObjectPools[rand])
		{
			// Check It is enemy
			ASTEnemyBase* Enemy = Cast<ASTEnemyBase>(Info.ObjectPools[rand]->GetCharacter<ASTEnemyBase>(SpawnNavLocation.Location));
			if (Enemy)
			{
				// Bind Function on Enemy Died!
				Enemy->Delegate_OnEnemyDied.RemoveDynamic(this, &UWorldEnemySpawnerComponent::OnEnemyDied);
				Enemy->Delegate_OnEnemyDied.AddDynamic(this, &UWorldEnemySpawnerComponent::OnEnemyDied);

				Info.CurrentSpawned++;
				CurrentSpawned++;
			}
			else
			{
				DrawDebugCapsule(GetWorld(), SpawnNavLocation.Location, 50.f, 25.f, FRotator::ZeroRotator.Quaternion(), FColor::Red, true);
			}
		}
	}
}

bool UWorldEnemySpawnerComponent::GetSpawnNavLocation(int infoIdx, FNavLocation& OutLocation) const
{
	const FSpawnInfo& Info = SpawnInfos[infoIdx];

	UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	if (!NavSystem) return false;

	NavSystem->GetRandomReachablePointInRadius(GetOwner()->GetActorLocation(), Info.SpawnableRadius_Outer, OutLocation);

	int maxLoopIdx = 50; int currentLoopIdx = 0;
	while (FVector::Dist2D(OutLocation.Location, GetOwner()->GetActorLocation()) < Info.SpawnableRadius_Inner && currentLoopIdx <= maxLoopIdx)
	{
		NavSystem->GetRandomReachablePointInRadius(GetOwner()->GetActorLocation(), Info.SpawnableRadius_Outer, OutLocation);
		currentLoopIdx++;
	}

	if (currentLoopIdx >= 50)
		UE_LOG(LogTemp, Warning, TEXT("USpawnEnemyComponent : Can't get Point between Inner and Outer Circle!"));

	return true;
}

void UWorldEnemySpawnerComponent::OnEnemyDied(AActor* DiedEnemy)
{
	// Called when enemy died
	if (DiedEnemy)
	{
		for (int i = 0; i < SpawnInfos.Num(); i++)
		{
			for (auto& SubClass : SpawnInfos[i].SpawnPawnClasses)
				if (DiedEnemy->GetClass() == SubClass.Get())
				{
					SpawnInfos[i].CurrentSpawned--;
				}
		}

		CurrentSpawned--;
	}
}


