// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionCondition/STMissionConditionBase.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/SpawnPoint/SpawnPointBase.h"

FText USTMissionConditionBase::GetConditionDescription_Implementation()
{
	return FText();
}

void USTMissionConditionBase::GetAllSpawnPointsWithTag(FGameplayTag InTag, TArray<ASpawnPointBase*>& OutActors) const
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPointBase::StaticClass(), actors);

	ASpawnPointBase* spawnPoint;
	for (const auto& actor : actors)
	{
		spawnPoint = Cast<ASpawnPointBase>(actor);
		if (spawnPoint && spawnPoint->HasTag(InTag))
		{
			OutActors.Add(spawnPoint);
		}
	}
}
