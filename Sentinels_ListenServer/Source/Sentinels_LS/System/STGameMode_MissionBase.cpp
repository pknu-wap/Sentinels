// Fill out your copyright notice in the Description page of Project Settings.


#include "System/STGameMode_MissionBase.h"
#include "System/STGameState.h"
#include "Actors/Section/STMissionSection.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

void ASTGameMode_MissionBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, ASTMissionSection::StaticClass(), Actors);

	if (Actors.Num() > 0)
	{
		CurrentMissionSection = Cast<ASTMissionSection>(Actors[UKismetMathLibrary::RandomIntegerInRange(0, Actors.Num() - 1)]);
	}
}

AActor* ASTGameMode_MissionBase::ChoosePlayerStart_Implementation(AController* Player)
{
	if (!CurrentMissionSection)
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(this, ASTMissionSection::StaticClass(), Actors);

		if (Actors.Num() > 0)
		{
			CurrentMissionSection = Cast<ASTMissionSection>(Actors[UKismetMathLibrary::RandomIntegerInRange(0, Actors.Num() - 1)]);
		}
	}

	if (CurrentMissionSection && CurrentMissionSection->PlayerStarts.Num() > 0)
	{
		return CurrentMissionSection->PlayerStarts[UKismetMathLibrary::RandomIntegerInRange(0, CurrentMissionSection->PlayerStarts.Num() - 1)];
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}

void ASTGameMode_MissionBase::BeginPlay()
{
	Super::BeginPlay();

	// Register All Missions
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, ASTMissionSection::StaticClass(), Actors);
	for (AActor* actor : Actors)
	{
		if (ASTMissionSection* section = Cast<ASTMissionSection>(actor))
		{
			section->RegisterRandomMission();
		}
	}

	// Activate Mission
	if (ASTGameState* gameState = Cast<ASTGameState>(UGameplayStatics::GetGameState(this)))
	{
		gameState->ActivateMission(CurrentMissionSection);
	}
}
