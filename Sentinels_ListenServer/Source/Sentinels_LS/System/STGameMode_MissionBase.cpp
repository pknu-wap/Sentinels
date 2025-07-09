// Fill out your copyright notice in the Description page of Project Settings.


#include "System/STGameMode_MissionBase.h"
#include "System/STGameState.h"
#include "Actors/Section/STMissionSection.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "STGameplayTags.h"

void ASTGameMode_MissionBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, ASTMissionSection::StaticClass(), Actors);

	for (int i = 0; i < Actors.Num(); i++)
	{
		if (ASTMissionSection* section = Cast<ASTMissionSection>(Actors[i]))
		{
			CachedMissionSections.Add(section);
		}
	}

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
}

void ASTGameMode_MissionBase::InitMissionInfos()
{
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

void ASTGameMode_MissionBase::OnMissionEnded(USTMissionBase* InMission, bool IsCleared)
{
	if (IsCleared)
	{
		// Teleport Players to Select Area
		bool bTeleportSuccess = false;
		for (int i = 0; i < CachedMissionSections.Num(); i++)
		{
			if (CachedMissionSections[i] && CachedMissionSections[i]->HasTag(FSTGameplayTags::Get().Section_SelectMissionOrBoss))
			{
				bTeleportSuccess = true;
				TeleportPlayersToPlayerStarts(CachedMissionSections[i]->PlayerStarts);
				break;
			}
		}

		if (bTeleportSuccess) 
		{
			// Set Timer for Determine
			GetWorldTimerManager().SetTimer(Handle_MissionOrBoss, this, &ASTGameMode_MissionBase::DetermineMissionOrBoss, 20.f, false);
		}
	}
}

void ASTGameMode_MissionBase::VoteToMission(const APlayerController* PC)
{
	BossVotedPlayers.Remove(PC->GetUniqueID());
	MissionVotedPlayers.Add(PC->GetUniqueID());

	/*int playerCount = GetNumPlayers();
	if (BossVotedPlayers.Num() + MissionVotedPlayers.Num() == playerCount)
	{
		DetermineMissionOrBoss();
	}*/
}

void ASTGameMode_MissionBase::VoteToBoss(const APlayerController* PC)
{
	MissionVotedPlayers.Remove(PC->GetUniqueID());
	BossVotedPlayers.Add(PC->GetUniqueID());
}

void ASTGameMode_MissionBase::DetermineMissionOrBoss()
{
	int playerCount = GetNumPlayers();

	bool bIsMission = MissionVotedPlayers.Num() > BossVotedPlayers.Num();
	if (MissionVotedPlayers.Num() == BossVotedPlayers.Num())
	{
		float randFloat = UKismetMathLibrary::RandomFloatInRange(0.f, 10.f);
		if (randFloat >= 5.f)
		{
			bIsMission = false;
		}
	}

	if (bIsMission)
	{
		// Mission

		// Start Random Misison
		if (ASTGameState* gameState = Cast<ASTGameState>(UGameplayStatics::GetGameState(this)))
		{
			gameState->ActivateRandomMission();

			// Teleport to Mission Section (PlayerStart)
			if (ASTMissionSection* missionSection = gameState->GetActivatedMissionSection())
			{
				TeleportPlayersToPlayerStarts(missionSection->PlayerStarts);
			}
		}
	}
	else
	{
		// Boss
	}
}

void ASTGameMode_MissionBase::TeleportPlayersToPlayerStarts(const TArray<APlayerStart*>& inPlayerStarts)
{
	int playerCount = GetNumPlayers();

	if (inPlayerStarts.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("ASTGameMode_MissionBase : Failed to get PlayerStarts from Mission Section!!!"));
		return;
	}

	for (int i = 0; i < inPlayerStarts.Num(); i++)
	{
		ACharacter* player = UGameplayStatics::GetPlayerCharacter(this, i);
		if (player)
		{
			player->SetActorTransform(inPlayerStarts[i]->GetTransform());
		}
	}

	if (inPlayerStarts.Num() < playerCount)
	{
		// numOfPlayerStart : 2 && numOfPlayer : 4
		// -> startIdx = 4 - 2 = 2
		int startIdx = playerCount - inPlayerStarts.Num();
		for (int idx = startIdx; idx < playerCount; idx++)
		{
			ACharacter* player = UGameplayStatics::GetPlayerCharacter(this, idx);
			if (player)
			{
				player->SetActorTransform(inPlayerStarts.Last()->GetTransform() + FTransform(FVector(100.f * idx, 0, 0)));
			}
		}
	}
}
