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
	if (!GetOwner()->HasAuthority())
		return;

	if (bShouldLoop)
	{
		if (!GetWorld()->GetTimerManager().TimerExists(handle))
			GetWorld()->GetTimerManager().SetTimer(handle, this, &USpawnEnemyComponent::SpawnEnemy, SpawnPeriod, true);
	}
	else
	{
		SpawnEnemy();
	}
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

	int rand; FNavLocation SpawnNavLocation;
	for (int i = 0; i < SpawnRate; i++)
	{
		if (CurrentSpawned >= MaxSpawnCount)
			break;

		// Check Get NavLocation Success && Check Class is valid 
		rand = UKismetMathLibrary::RandomIntegerInRange(0, SpawnPawnClasses.Num() - 1);
		if (!GetSpawnNavLocation(SpawnNavLocation) || !SpawnPawnClasses[rand])
			continue;

		// Spawn Enemy
		SpawnNavLocation.Location.Z += 75.f;
		APawn* pawn = UAIBlueprintHelperLibrary::SpawnAIFromClass(this, SpawnPawnClasses[rand], nullptr,
			SpawnNavLocation.Location, GetOwner()->GetActorRotation());

		// Check It is enemy
		ASTEnemyBase* Enemy = Cast<ASTEnemyBase>(pawn);
		if (Enemy)
		{
			// Set Target for enemy
			SetTarget(Enemy);
			
			/*FTimerHandle TargetHandle;
			GetWorld()->GetTimerManager().SetTimer(TargetHandle, [Enemy, this]() { this->SetTarget(Enemy); }, 1.5f, false);*/
			
			
			// Bind Function on Enemy Died!
			Enemy->Delegate_OnEnemyDied.AddDynamic(this, &USpawnEnemyComponent::OnEnemyDied);
			CurrentSpawned++;
			SpawnedEnemys.Push(Enemy);
		}
		else
		{
			DrawDebugCapsule(GetWorld(), SpawnNavLocation.Location, 50.f, 25.f, FRotator::ZeroRotator.Quaternion(), FColor::Red, true);
		}
	}
}

bool USpawnEnemyComponent::GetSpawnNavLocation(FNavLocation& OutLocation) const
{
	UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	if (!NavSystem) return false;

	NavSystem->GetRandomReachablePointInRadius(GetOwner()->GetActorLocation(), SpawnableRadius_Outer, OutLocation);

	int maxLoopIdx = 50; int currentLoopIdx = 0;
	while (FVector::Dist2D(OutLocation.Location, GetOwner()->GetActorLocation()) < SpawnableRadius_Inner && currentLoopIdx <= maxLoopIdx)
	{
		NavSystem->GetRandomReachablePointInRadius(GetOwner()->GetActorLocation(), SpawnableRadius_Outer, OutLocation);
		currentLoopIdx++;
	}

	if (currentLoopIdx >= 50)
		UE_LOG(LogTemp, Warning, TEXT("USpawnEnemyComponent : Can't get Point between Inner and Outer Circle!"));

	return true;
}

void USpawnEnemyComponent::SetTarget(ASTEnemyBase* inEnemy)
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
			controller->SetTarget(GetRandomPlayerCharacter());
		}
	}
}

ACharacter* USpawnEnemyComponent::GetRandomPlayerCharacter() const
{
	ACharacter* player = nullptr;

	int playerNum = UGameplayStatics::GetNumPlayerControllers(this);
	int rand = UKismetMathLibrary::RandomIntegerInRange(0, playerNum - 1);
	player = UGameplayStatics::GetPlayerCharacter(this, rand);

	return player;
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

