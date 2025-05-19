// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Spawner/WorldBoxAISpawner.h"
#include "Components/BoxComponent.h"
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

// Sets default values
AWorldBoxAISpawner::AWorldBoxAISpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(FName("BoxComponent"));
	Box->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AWorldBoxAISpawner::BeginPlay()
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

// Called every frame
void AWorldBoxAISpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorldBoxAISpawner::StartSpawnEnemy()
{
	if (!HasAuthority())
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

void AWorldBoxAISpawner::StopSpawnEnemy()
{
	for (int i = 0; i < SpawnInfos.Num(); i++)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnInfos[i].TimerHandle);
	}
}

void AWorldBoxAISpawner::SpawnEnemy(int InfoIdx)
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
				Enemy->SetAdditionalDropInfos(Info.AdditionalDropInfos);

				// Bind Function on Enemy Died!
				Enemy->Delegate_OnEnemyDied.RemoveDynamic(this, &AWorldBoxAISpawner::OnEnemyDied);
				Enemy->Delegate_OnEnemyDied.AddDynamic(this, &AWorldBoxAISpawner::OnEnemyDied);

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

bool AWorldBoxAISpawner::IsVectorInBoundingBox(const FVector& InLocation) const
{
	FVector BaseLocation = GetActorLocation();
	FVector BoxExtent = Box->GetScaledBoxExtent();

	if ((BaseLocation.X - BoxExtent.X < InLocation.X && InLocation.X < BaseLocation.X + BoxExtent.X)
		&& (BaseLocation.Y - BoxExtent.Y < InLocation.Y && InLocation.Y < BaseLocation.Y + BoxExtent.Y)
		&& (BaseLocation.Z - BoxExtent.Z < InLocation.Z && InLocation.Z < BaseLocation.Z + BoxExtent.Z))
	{
		UE_LOG(LogTemp, Display, TEXT("IsVectorInBoundingBox : true"));
		return true;
	}

	UE_LOG(LogTemp, Display, TEXT("InLocation : %s"), *InLocation.ToString());
	UE_LOG(LogTemp, Display, TEXT("IsVectorInBoundingBox : false"));
	return false;
}

bool AWorldBoxAISpawner::GetSpawnNavLocation(int infoIdx, FNavLocation& OutLocation) const
{
	const FSpawnInfo& Info = SpawnInfos[infoIdx];

	UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	if (!NavSystem) return false;

	if (!NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), Info.SpawnableRadius_Outer, OutLocation))
	{
		UE_LOG(LogTemp, Warning, TEXT("AWorldBoxAISpawner : Failed to get spawnable Nav Location!"));
		return false;
	}

	int maxLoopIdx = 50; int currentLoopIdx = 0;
	while (
		!(FVector::Dist2D(OutLocation.Location, GetActorLocation()) >= Info.SpawnableRadius_Inner 
		&& IsVectorInBoundingBox(OutLocation.Location))
		&& currentLoopIdx <= maxLoopIdx)
	{
		NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), Info.SpawnableRadius_Outer, OutLocation);
		currentLoopIdx++;
	}

	if (currentLoopIdx >= 50)
	{
		UE_LOG(LogTemp, Warning, TEXT("USpawnEnemyComponent : Can't get Point between Inner and Outer Circle!"));
		return false;
	}

	return true;
}

bool AWorldBoxAISpawner::GetSpawnNavLocationInBox(int infoIdx, FNavLocation& OutLocation) const
{
	return false;
}

void AWorldBoxAISpawner::OnEnemyDied(AActor* DiedEnemy)
{
	UE_LOG(LogTemp, Display, TEXT("AWorldBoxAISpawner::OnEnemyDied"));
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

