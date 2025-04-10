// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionCondition/DefenseWave/STDefenseWaveCondition.h"
#include "System/STGameState.h"
#include "System/Mission/STMissionBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/MissionObject/NonInteractable/DefenseCore/DefenseCore.h"
#include "Actors/SpawnPoint/SpawnPointBase.h"
#include "STGameplayTags.h"

void USTDefenseWaveCondition::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

bool USTDefenseWaveCondition::IsSatisfied()
{
	return false;
}

void USTDefenseWaveCondition::MissionActivated()
{
	// Set Time Limit
	GetWorld()->GetTimerManager().SetTimer(
		handle, this, &USTDefenseWaveCondition::TimeLimitEnded, DominationTimeLimit, false);

	// Spawn Domination Core
	FVector SpawnLocation; FRotator SpawnRotation;

	TArray<ASpawnPointBase*> SpawnPoints;
	GetAllSpawnPointsWithTag(FSTGameplayTags::Get().SpawnPoint_DefenseCore, SpawnPoints);

	if (SpawnPoints.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("USTDefenseWaveCondition::MissionActivated Failed to get SpawnPoints of Defense Core!"));
		return;
	}

	int Rand = UKismetMathLibrary::RandomIntegerInRange(0, SpawnPoints.Num() - 1);
	SpawnLocation = SpawnPoints[Rand]->GetActorLocation();
	SpawnRotation = SpawnPoints[Rand]->GetActorRotation();

	Core = GetWorld()->SpawnActor<ADefenseCore>(SubclassOfDefenseCore, SpawnLocation, SpawnRotation);
	if (Core)
	{
		Core->Delegate_MissionConditionUpdate.AddUObject(this, &USTDefenseWaveCondition::ConditionUpdated);
	}
}

void USTDefenseWaveCondition::MissionDeactivated(bool IsCleared)
{

}

void USTDefenseWaveCondition::ConditionUpdated(int ObjectID, bool Success)
{
	if (Success) return;

	// Stop Spawn Enemy
	if (Core)
	{
		Core->StopSpawnEnemy();
	}

	// Defense Core is Broken
	ASTGameState* GameState = Cast<ASTGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		USTMissionBase* Mission = GameState->GetMission(MissionTag);
		if (Mission)
		{
			Mission->DeactivateMission(false);
		}
	}
}

void USTDefenseWaveCondition::TimeLimitEnded()
{
	// Stop Spawn Enemy
	if (Core)
	{
		Core->StopSpawnEnemy();
	}

	// Time Limit Success
	ASTGameState* GameState = Cast<ASTGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		USTMissionBase* Mission = GameState->GetMission(MissionTag);
		if (Mission)
		{
			Mission->DeactivateMission(true);
		}
	}
}

