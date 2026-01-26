// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MissionObject/Interactable/EscortObject/EscortObject.h"
#include "Actors/SplineRoute/SplineRouteActor.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SpawnEnemyComponent.h"
#include "SubSystem/STAIPoolingWorldSubsystem.h"
#include "SubSystem/STWorldSpawnSubsystem.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/Enemy/STEnemyBase_AIController.h"
#include "Player/STPlayerCharacter.h"
#include "Character/Enemy/STEnemyBase.h"

AEscortObject::AEscortObject()
{
	bReplicates = true;
	SetReplicateMovement(true);

	PrimaryActorTick.bCanEverTick = true;

	SpawnEnemyComp = CreateDefaultSubobject<USpawnEnemyComponent>(FName("SpawnComp"));

	BoxComp = CreateDefaultSubobject<UBoxComponent>(FName("BoxComponent"));
	BoxComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	SpawnLocationSceneComponent = CreateDefaultSubobject<USceneComponent>(FName("Spare Spawn Location"));
	SpawnLocationSceneComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
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

				bIsSuccessed = true;
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

		int numOfPlayers = UGameplayStatics::GetNumPlayerControllers(this);
		if (numOfPlayers > 0)
		{
			for (int i = 0; i < numOfPlayers; i++)
			{
				ACharacter* player = UGameplayStatics::GetPlayerCharacter(this, i);
				if (player)
					Players.Push(player);
			}
		}

		GetWorldTimerManager().SetTimer(Handle_SpawnWave, this, &AEscortObject::SpawnWave, WavePeriod, false);

		// Enable Spawn Enemy while Escort Mission
		// SpawnEnemyComp->StartSpawnEnemy();
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
	if (ASTEnemyBase* enemy = Cast<ASTEnemyBase>(OtherActor))
	{
		OverlappedEnemys.Add(enemy);
	}
	else if(ASTPlayerCharacter* player = Cast<ASTPlayerCharacter>(OtherActor))
	{
		OverlappedPlayers.Add(player);
	}

	if (OverlappedEnemys.Num() <= 5 && OverlappedPlayers.Num() > 0)
	{
		// StartMoveAlongSpline();
	}
}

void AEscortObject::BoxEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ASTEnemyBase* enemy = Cast<ASTEnemyBase>(OtherActor))
	{
		OverlappedEnemys.Remove(enemy);
	}
	else if (ASTPlayerCharacter* player = Cast<ASTPlayerCharacter>(OtherActor))
	{
		OverlappedPlayers.Remove(player);
	}

	if (OverlappedEnemys.Num() > 5 || OverlappedPlayers.Num() == 0)
	{
		// StopMove();
	}
}

void AEscortObject::SpawnWave()
{
	USTAIPoolingWorldSubsystem* PoolingSystem = GetWorld()->GetSubsystem<USTAIPoolingWorldSubsystem>();
	USTWorldSpawnSubsystem* SpawnSystem = GetWorld()->GetSubsystem<USTWorldSpawnSubsystem>();
	UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	if (!PoolingSystem || !SpawnSystem || !NavSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("AEscortObject : Failed to get Nagivation?!"));
		return;
	}

	bShouldMove = false;

	for (int InfoIdx = 0; InfoIdx < SpawnInfos.Num(); InfoIdx++)
	{
		FSpawnInfo& Info = SpawnInfos[InfoIdx];
		if (Info.SpawnPawnClasses.Num() == 0)
			continue;

		int rand; FNavLocation SpawnNavLocation;
		for (int i = 0; i < Info.MaxSpawnCount; i++)
		{
			if (Info.CurrentSpawned >= Info.MaxSpawnCount)
				break;

			// Check Get NavLocation Success && Check Class is valid 
			int playerIdx = UKismetMathLibrary::RandomIntegerInRange(0, Players.Num() - 1);
			rand = UKismetMathLibrary::RandomIntegerInRange(0, Info.SpawnPawnClasses.Num() - 1);
			if (!GetSpawnNavLocationForPlayer(playerIdx, InfoIdx, SpawnNavLocation) || !Info.SpawnPawnClasses[rand])
				continue;

			// Spawn Enemy
			SpawnNavLocation.Location.Z += 75.f;

			// Check It is enemy
			ASTEnemyBase* Enemy = PoolingSystem->GetCharacter<ASTEnemyBase>(Info.SpawnPawnClasses[rand], SpawnNavLocation.Location);
			if (Enemy)
			{
				Info.SpawnedCharacters.Add(Enemy);

				Enemy->SetAdditionalDropInfos(Info.AdditionalDropInfos);

				ASTEnemyBase_AIController* controller = Cast<ASTEnemyBase_AIController>(Enemy->GetController());
				if (controller)
					controller->SetTarget(Players[playerIdx]);

				// Bind Function on Enemy Died!
				Enemy->Delegate_OnEnemyDied.RemoveAll(this);
				Enemy->Delegate_OnEnemyDied.AddDynamic(this, &AEscortObject::OnEnemyDied);

				Info.CurrentSpawned++;
				// SpawnSystem->NewCharacterSpawned(Enemy);

				DrawDebugCapsule(GetWorld(), SpawnNavLocation.Location, 50.f, 25.f, FRotator::ZeroRotator.Quaternion(), FColor::Green, true);
			}
			else
			{
				DrawDebugCapsule(GetWorld(), SpawnNavLocation.Location, 50.f, 25.f, FRotator::ZeroRotator.Quaternion(), FColor::Red, true);
			}

		}
	}
}

bool AEscortObject::IsInFrontalCone(const FVector& locationToCheck, const FVector& originLocation, const FVector& forwardVector, float angleDeg) const
{
	FVector TowardVec = locationToCheck - originLocation;
	float DotProductResult = FVector::DotProduct(forwardVector, TowardVec);
	DotProductResult = FMath::Clamp(DotProductResult, -1.0f, 1.0f);

	float CosHalfAngle = FMath::Cos(FMath::DegreesToRadians(67.5f));
	return DotProductResult >= CosHalfAngle;
}

bool AEscortObject::GetSpawnNavLocationForPlayer(int playerIdx, int infoIdx, FNavLocation& OutLocation) const
{
	if (Players.Num() == 0)
		return false;

	AActor* player = Players[playerIdx];
	if (!IsValid(player))
	{
		player = UGameplayStatics::GetPlayerCharacter(this, playerIdx);
		if (!player) return false;
	}

	const FSpawnInfo& Info = SpawnInfos[infoIdx];

	UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	if (!NavSystem) return false;

	FVector baseLocation = player->GetActorLocation();
	if (!NavSystem->GetRandomReachablePointInRadius(baseLocation, Info.SpawnableRadius_Outer, OutLocation))
	{
		UE_LOG(LogTemp, Warning, TEXT("AWorldBoxAISpawner : Failed to get spawnable Nav Location!"));
		baseLocation = SpawnLocationSceneComponent->GetComponentLocation();
		// return false;
	}

	int maxLoopIdx = 50; int currentLoopIdx = 0;
	while (currentLoopIdx <= maxLoopIdx)
	{
		NavSystem->GetRandomReachablePointInRadius(baseLocation, Info.SpawnableRadius_Outer, OutLocation);

		if (FVector::Dist2D(OutLocation.Location, baseLocation) >= Info.SpawnableRadius_Inner
			&& IsInFrontalCone(OutLocation.Location, baseLocation, player->GetActorForwardVector(), 135.f))
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

void AEscortObject::OnEnemyDied(AActor* DiedEnemy)
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
			SpawnInfos[i].SpawnedCharacters.Remove(Enemy);
			for (auto& SubClass : SpawnInfos[i].SpawnPawnClasses)
				if (DiedEnemy->GetClass() == SubClass.Get())
				{
					SpawnInfos[i].CurrentSpawned--;
				}
		}
	}
	
	// Check Wave is cleared
	bool bIsWaveCleared = true;
	for (int i = 0; i < SpawnInfos.Num(); i++)
	{
		if (SpawnInfos[i].CurrentSpawned > 0)
		{
			bIsWaveCleared = false;
			break;
		}
	}

	// if wave is cleared, Spawn Wave again
	if (!bIsSuccessed && bIsWaveCleared)
	{
		bShouldMove = true;
		GetWorldTimerManager().SetTimer(Handle_SpawnWave, this, &AEscortObject::SpawnWave, WavePeriod, false);
	}
}