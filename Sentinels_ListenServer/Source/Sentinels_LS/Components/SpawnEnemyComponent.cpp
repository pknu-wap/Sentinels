// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SpawnEnemyComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/Enemy/STEnemyBase_AIController.h"
#include "Character/Enemy/STEnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/ObjectPool/CharacterObjectPool.h"

// Sets default values for this component's properties
USpawnEnemyComponent::USpawnEnemyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void USpawnEnemyComponent::BeginPlay()
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

void USpawnEnemyComponent::StartSpawnEnemy()
{
	if (!GetOwner()->HasAuthority())
		return;

	for (int i = 0; i < SpawnInfos.Num(); i++)
	{
		if (!GetWorld()->GetTimerManager().TimerExists(SpawnInfos[i].TimerHandle))
		{
			DrawDebugCircle(GetWorld(), GetOwner()->GetTransform().ToMatrixNoScale(), SpawnInfos[i].SpawnableRadius_Outer, 16, FColor::Green, true);
			DrawDebugCircle(GetWorld(), GetOwner()->GetTransform().ToMatrixNoScale(), SpawnInfos[i].SpawnableRadius_Inner, 16, FColor::Green, true);

			GetWorld()->GetTimerManager().SetTimer(SpawnInfos[i].TimerHandle,
				[this, i]() 
				{
					SpawnEnemy(i);
				}
			, SpawnInfos[i].SpawnPeriod, true);
		}
	}
}

void USpawnEnemyComponent::StopSpawnEnemy()
{
	for (int i = 0; i < SpawnInfos.Num(); i++)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnInfos[i].TimerHandle);
	}
}

void USpawnEnemyComponent::SpawnEnemy(int InfoIdx)
{
	if (!SpawnInfos.IsValidIndex(InfoIdx))
	{
		return;
	}

	FSpawnInfo& Info = SpawnInfos[InfoIdx];

	if (Info.SpawnPawnClasses.Num() == 0)
	{
		UE_LOG(LogTemp, Display, TEXT("USpawnEnemyComponent : Pawn Classes are not setted!"));
		return;
	}

	int rand; FNavLocation SpawnNavLocation;
	for (int i = 0; i < Info.SpawnRate; i++)
	{
		if (Info.CurrentSpawned >= Info.MaxSpawnCount)
		{
			if (!bShouldLoop)
			{
				GetWorld()->GetTimerManager().ClearTimer(Info.TimerHandle);
			}

			break;
		}

		// Check Get NavLocation Success && Check Class is valid 
		rand = UKismetMathLibrary::RandomIntegerInRange(0, Info.SpawnPawnClasses.Num() - 1);
		if (!GetSpawnNavLocation(InfoIdx, SpawnNavLocation) || !Info.SpawnPawnClasses[rand])
			continue;

		// Spawn Enemy
		SpawnNavLocation.Location.Z += 75.f;

		if (Info.ObjectPools[rand])
		{
			// Check It is enemy
			ASTEnemyBase* Enemy = Info.ObjectPools[rand]->GetCharacter<ASTEnemyBase>(SpawnNavLocation.Location);
			if (Enemy)
			{
				// Set Target for enemy
				SetTarget(InfoIdx, Enemy);

				// Bind Function on Enemy Died!
				Enemy->Delegate_OnEnemyDied.RemoveDynamic(this, &USpawnEnemyComponent::OnEnemyDied);
				Enemy->Delegate_OnEnemyDied.AddDynamic(this, &USpawnEnemyComponent::OnEnemyDied);
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

bool USpawnEnemyComponent::GetSpawnNavLocation(int InfoIdx, FNavLocation& OutLocation) const
{
	UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	if (!NavSystem) return false;

	NavSystem->GetRandomReachablePointInRadius(GetOwner()->GetActorLocation(), SpawnInfos[InfoIdx].SpawnableRadius_Outer, OutLocation);

	int maxLoopIdx = 50; int currentLoopIdx = 0;
	while (FVector::Dist2D(OutLocation.Location, GetOwner()->GetActorLocation()) < SpawnInfos[InfoIdx].SpawnableRadius_Inner && currentLoopIdx <= maxLoopIdx)
	{
		NavSystem->GetRandomReachablePointInRadius(GetOwner()->GetActorLocation(), SpawnInfos[InfoIdx].SpawnableRadius_Outer, OutLocation);
		currentLoopIdx++;
	}

	if (currentLoopIdx >= 50)
	{
		UE_LOG(LogTemp, Warning, TEXT("USpawnEnemyComponent : Can't get Point between Inner and Outer Circle!"));
		return false;
	}

	return true;
}

void USpawnEnemyComponent::SetTarget(int InfoIdx, ASTEnemyBase* inEnemy)
{
	if (!inEnemy)
		return;
	
	ASTEnemyBase_AIController* controller = Cast<ASTEnemyBase_AIController>(inEnemy->GetController());
	if (controller)
	{
		if (bShouldTargetOwner)
		{
			controller->SetTarget(GetOwner());
		}
		else
		{
			controller->SetTarget(GetRandomPlayerCharacter(InfoIdx, inEnemy));
		}
	}
}

ACharacter* USpawnEnemyComponent::GetRandomPlayerCharacter(int InfoIdx, ASTEnemyBase* inEnemy) const
{
	ACharacter* player = nullptr;

	int playerNum = UGameplayStatics::GetNumPlayerControllers(this);

	// find near player
	TArray<ACharacter*> nearPlayers;
	ACharacter* randomPlayer = nullptr;
	for (int i = 0; i < playerNum; i++)
	{
		randomPlayer = UGameplayStatics::GetPlayerCharacter(this, i);
		if (randomPlayer)
		{
			float dist = FVector::Dist2D(inEnemy->GetActorLocation(), randomPlayer->GetActorLocation());
			if (dist <= SpawnInfos[InfoIdx].MaxDistanceToPlayer)
				nearPlayers.Push(randomPlayer);
		}
	}

	// Failed to find near player
	if (nearPlayers.Num() == 0)
		return nullptr;

	int rand = UKismetMathLibrary::RandomIntegerInRange(0, nearPlayers.Num() - 1);
	player = nearPlayers[rand];

	return player;
}

void USpawnEnemyComponent::OnEnemyDied(AActor* DiedEnemy)
{
	// Called when enemy died
	if (DiedEnemy)
	{
		for (int i = 0; i < SpawnInfos.Num(); i++)
		{
			for(auto& SubClass : SpawnInfos[i].SpawnPawnClasses)
				if (DiedEnemy->GetClass() == SubClass.Get())
				{
					SpawnInfos[i].CurrentSpawned--;
					// SpawnInfos[i].SpawnedEnemys.Remove(DiedEnemy);
				}
		}

		CurrentSpawned--;
	}

	if (CurrentSpawned <= 0)
	{
		Delegate_OnEnemyAllDied.Broadcast();
	}
}

