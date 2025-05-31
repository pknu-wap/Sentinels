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
#include "Player/STPlayerCharacter.h"
#include "SubSystem/STAIPoolingWorldSubsystem.h"
#include "SubSystem/STWorldSpawnSubsystem.h"

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

	USTAIPoolingWorldSubsystem* PoolingSystem = GetWorld()->GetSubsystem<USTAIPoolingWorldSubsystem>();
	if (!PoolingSystem) return;

	if (HasAuthority())
	{
		NumOfOverlappedPlayers = 0;
		Box->OnComponentBeginOverlap.AddDynamic(this, &AWorldBoxAISpawner::BoxBeginOverlapped);
		Box->OnComponentEndOverlap.AddDynamic(this, &AWorldBoxAISpawner::BoxEndOverlapped);

		for (auto& SpawnInfo : SpawnInfos)
		{
			SpawnInfo.ObjectPools.Reserve(SpawnInfo.SpawnPawnClasses.Num());

			for (int i = 0; i < SpawnInfo.SpawnPawnClasses.Num(); i++)
			{
				PoolingSystem->InitCharacterPool(SpawnInfo.SpawnPawnClasses[i], SpawnInfo.MaxSpawnCount);
				
				/*ACharacterObjectPool* ObjectPool =
					GetWorld()->SpawnActor<ACharacterObjectPool>(ACharacterObjectPool::StaticClass());

				if (ObjectPool)
				{
					ObjectPool->InitPool(SpawnInfo.SpawnPawnClasses[i], SpawnInfo.MaxSpawnCount);
					SpawnInfo.ObjectPools.Add(ObjectPool);
				}*/
			}
		}
	}
}

void AWorldBoxAISpawner::BoxBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASTPlayerCharacter* Player = Cast<ASTPlayerCharacter>(OtherActor))
	{
		if (NumOfOverlappedPlayers == 0)
		{
			StartSpawnEnemy();
		}

		NumOfOverlappedPlayers++;
		Players.Add(Player);
	}

}

void AWorldBoxAISpawner::BoxEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ASTPlayerCharacter* Player = Cast<ASTPlayerCharacter>(OtherActor))
	{
		NumOfOverlappedPlayers--;
		Players.Remove(Player);

		if (NumOfOverlappedPlayers == 0)
		{
			StopSpawnEnemy();
		}
	}
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
					if (!SpawnReserveSet.Contains(i))
					{
						SpawnReserveSet.Add(i);
						SpawnReserveQue.Enqueue(i);
					}
				}
			, SpawnInfos[i].SpawnPeriod, true);
		}
	}

	FTimerHandle SpawnHandle;
	GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &AWorldBoxAISpawner::TrySpawnAI, 1.f, true);
}

void AWorldBoxAISpawner::StopSpawnEnemy()
{
	for (int i = 0; i < SpawnInfos.Num(); i++)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnInfos[i].TimerHandle);
	}
}

void AWorldBoxAISpawner::TrySpawnAI()
{
	if (!SpawnReserveQue.IsEmpty())
	{
		int idx = 0;
		SpawnReserveQue.Dequeue(idx); SpawnReserveSet.Remove(idx);
		SpawnEnemy(idx);
	}
}

void AWorldBoxAISpawner::SpawnEnemy(int InfoIdx)
{
	USTAIPoolingWorldSubsystem* PoolingSystem = GetWorld()->GetSubsystem<USTAIPoolingWorldSubsystem>();
	USTWorldSpawnSubsystem* SpawnSystem = GetWorld()->GetSubsystem<USTWorldSpawnSubsystem>();
	if (!PoolingSystem || !SpawnSystem || !SpawnInfos.IsValidIndex(InfoIdx) || Players.Num() == 0)
	{
		return;
	}

	FSpawnInfo& Info = SpawnInfos[InfoIdx];
	if (Info.SpawnPawnClasses.Num() == 0) return;

	int rand; FNavLocation SpawnNavLocation;
	for (int i = 0; i < Info.SpawnRate; i++)
	{
		if (Info.CurrentSpawned >= Info.MaxSpawnCount)
			break;

		if (!SpawnSystem->CanSpawnCharacter())
		{
			break;
		}

		int playerIdx = UKismetMathLibrary::RandomIntegerInRange(0, Players.Num() - 1);

		// Check Get NavLocation Success && Check Class is valid 
		rand = UKismetMathLibrary::RandomIntegerInRange(0, Info.SpawnPawnClasses.Num() - 1);
		if (!GetSpawnNavLocationForPlayer(playerIdx, InfoIdx, SpawnNavLocation) || !Info.SpawnPawnClasses[rand])
			continue;

		// Spawn Enemy
		SpawnNavLocation.Location.Z += 75.f;

		// Check It is enemy
		ASTEnemyBase* Enemy = PoolingSystem->GetCharacter<ASTEnemyBase>(Info.SpawnPawnClasses[rand], SpawnNavLocation.Location);
		if (Enemy)
		{
			Enemy->SetAdditionalDropInfos(Info.AdditionalDropInfos);

			ASTEnemyBase_AIController* controller = Cast<ASTEnemyBase_AIController>(Enemy->GetController());
			if (controller)
				controller->SetTarget(Players[playerIdx]);

			// Bind Function on Enemy Died!
			Enemy->Delegate_OnEnemyDied.RemoveAll(this);
			Enemy->Delegate_OnEnemyDied.AddDynamic(this, &AWorldBoxAISpawner::OnEnemyDied);

			Info.CurrentSpawned++;
			CurrentSpawned++;
			SpawnSystem->NewCharacterSpawned(Enemy);
		}
		else
		{
			DrawDebugCapsule(GetWorld(), SpawnNavLocation.Location, 50.f, 25.f, FRotator::ZeroRotator.Quaternion(), FColor::Red, true);
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

bool AWorldBoxAISpawner::IsInFrontalCone(const FVector& locationToCheck, const FVector& originLocation, const FVector& forwardVector, float angleDeg) const
{
	FVector TowardVec = locationToCheck - originLocation;
	float DotProductResult = FVector::DotProduct(forwardVector, TowardVec);
	DotProductResult = FMath::Clamp(DotProductResult, -1.0f, 1.0f);

	float CosHalfAngle = FMath::Cos(FMath::DegreesToRadians(67.5f));
	return DotProductResult >= CosHalfAngle;
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

bool AWorldBoxAISpawner::GetSpawnNavLocationForPlayer(int playerIdx, int infoIdx, FNavLocation& OutLocation) const
{
	if (Players.Num() == 0)
		return false;

	AActor* player = Players[playerIdx];
	
	const FSpawnInfo& Info = SpawnInfos[infoIdx];

	UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	if (!NavSystem) return false;

	if (!NavSystem->GetRandomReachablePointInRadius(player->GetActorLocation(), Info.SpawnableRadius_Outer, OutLocation))
	{
		UE_LOG(LogTemp, Warning, TEXT("AWorldBoxAISpawner : Failed to get spawnable Nav Location!"));
		return false;
	}

	int maxLoopIdx = 50; int currentLoopIdx = 0;
	while (currentLoopIdx <= maxLoopIdx)
	{
		NavSystem->GetRandomReachablePointInRadius(player->GetActorLocation(), Info.SpawnableRadius_Outer, OutLocation);
		
		if (FVector::Dist2D(OutLocation.Location, player->GetActorLocation()) >= Info.SpawnableRadius_Inner
			&& IsInFrontalCone(OutLocation.Location, player->GetActorLocation(), player->GetActorForwardVector(), 135.f))
			break;
		
		currentLoopIdx++;
	}

	if (currentLoopIdx >= 50)
	{
		UE_LOG(LogTemp, Warning, TEXT("USpawnEnemyComponent : Can't get Point between Inner and Outer Circle!"));
		return false;
	}

	return true;
}

void AWorldBoxAISpawner::OnEnemyDied(AActor* DiedEnemy)
{
	USTWorldSpawnSubsystem* SpawnSystem = GetWorld()->GetSubsystem<USTWorldSpawnSubsystem>();
	if (SpawnSystem)
	{
		SpawnSystem->CharacterDeactivated(DiedEnemy);
	}

	UE_LOG(LogTemp, Display, TEXT("AWorldBoxAISpawner::OnEnemyDied"));
	// Called when enemy died
	ASTEnemyBase* Enemy = Cast<ASTEnemyBase>(DiedEnemy);
	if (Enemy)
	{
		Enemy->Delegate_OnEnemyDied.RemoveAll(this);
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

