// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionCondition/Domination/STDominationCondition.h"
#include "System/STGameState.h"
#include "System/Mission/STMissionBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/SpawnPoint/SpawnPoint_DominationCore.h"
#include "Actors/Interact/DominationCore/InteractableDominationCore.h"

void USTDominationCondition::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

bool USTDominationCondition::IsSatisfied()
{
	return false;
}

void USTDominationCondition::MissionActivated()
{
	// Set Time Limit
	GetWorld()->GetTimerManager().SetTimer(
		handle, this, &USTDominationCondition::TimeLimitEnded, DominationTimeLimit, false);

	// Spawn Domination Core
	FVector SpawnLocation; FRotator SpawnRotation;

	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SubclassOfSpawnPoint, SpawnPoints);

	int Rand = UKismetMathLibrary::RandomIntegerInRange(0, SpawnPoints.Num() - 1);
	SpawnLocation = SpawnPoints[Rand]->GetActorLocation(); 
	SpawnRotation = SpawnPoints[Rand]->GetActorRotation();

	GetWorld()->SpawnActor<AInteractableDominationCore>(SubclassOfDominatinoCore, SpawnLocation, SpawnRotation);
}

void USTDominationCondition::MissionDeactivated(bool IsCleared)
{

}

void USTDominationCondition::UpdateDominationInfo(int ObjectID, bool Success)
{
	// Domination Core is Broken
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

void USTDominationCondition::TimeLimitEnded()
{
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
