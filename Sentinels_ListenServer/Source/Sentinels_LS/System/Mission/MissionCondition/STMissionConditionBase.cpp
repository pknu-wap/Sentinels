// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionCondition/STMissionConditionBase.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/SpawnPoint/SpawnPointBase.h"
#include "Actors/Section/STMissionSection.h"
#include "Kismet/KismetMathLibrary.h"

FText USTMissionConditionBase::GetConditionDescription_Implementation()
{
	return FText();
}

void USTMissionConditionBase::GetAllSpawnPointsWithTag(FGameplayTag InTag, TArray<ASpawnPointBase*>& OutActors) const
{
	TArray<ASTMissionSection*> sections;

	/*
		Find All Sections With Tag (Not Selected yet)
	*/
	TArray<AActor*> sectionActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASTMissionSection::StaticClass(), sectionActors);
	ASTMissionSection* section = nullptr;
	for (const auto& actor : sectionActors)
	{
		section = Cast<ASTMissionSection>(actor);

		// check has tag && not selected
		if (section && section->HasTag(InTag) && !section->bIsSelected)
		{
			sections.Add(section);
		}
	}

	/*
		Find All SpawnPoints with Tag in Section
	*/
	int rand = UKismetMathLibrary::RandomIntegerInRange(0, sections.Num() - 1);
	if (sections.IsValidIndex(rand) && sections[rand])
	{
		sections[rand]->bIsSelected = true;

		TArray<AActor*> SectionChildActors;
		sections[rand]->GetAllChildActors(SectionChildActors);

		ASpawnPointBase* spawnPoint = nullptr;
		for (const auto& actor : SectionChildActors)
		{
			spawnPoint = Cast<ASpawnPointBase>(actor);
			if (spawnPoint && spawnPoint->HasTag(InTag))
			{
				OutActors.Add(spawnPoint);
			}
		}
	}


	/*TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPointBase::StaticClass(), actors);

	ASpawnPointBase* spawnPoint;
	for (const auto& actor : actors)
	{
		spawnPoint = Cast<ASpawnPointBase>(actor);
		if (spawnPoint && spawnPoint->HasTag(InTag))
		{
			OutActors.Add(spawnPoint);
		}
	}*/
}

void USTMissionConditionBase::MissionRegistered_Implementation()
{

}

void USTMissionConditionBase::MissionActivated_Implementation()
{

}

void USTMissionConditionBase::MissionDeactivated_Implementation(bool IsCleared)
{

}

bool USTMissionConditionBase::IsSatisfied_Implementation()
{
	return false;
}