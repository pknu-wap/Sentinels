// Fill out your copyright notice in the Description page of Project Settings.


#include "System/STGameMode_Roguelite.h"
#include "System/STGameState.h"
#include "Actors/Section/STMissionSection.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Player/STPlayerCharacter.h"
#include "STGameplayTags.h"
#include "Character/Enemy/STEliteBase.h"

void ASTGameMode_Roguelite::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	/*
		Caching All Sections
	*/
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, ASTMissionSection::StaticClass(), Actors);

	for (int i = 0; i < Actors.Num(); i++)
	{
		if (ASTMissionSection* section = Cast<ASTMissionSection>(Actors[i]))
		{
			CachedSections.Add(section);
		}
	}

	// Shuffle Section!
	ShuffleTArray(CachedSections);

	CurrentSectionIdx = -1;
}

void ASTGameMode_Roguelite::PreInitializeComponents()
{
	// GameState is created at AGameModeBase::PreInitializeComponents
	Super::PreInitializeComponents();

	// Activate Random Section (Array is Shuffled on InitGame)
	ActivateNextSection();
}

AActor* ASTGameMode_Roguelite::ChoosePlayerStart_Implementation(AController* Player)
{
	if (!CurrentSection)
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(this, ASTMissionSection::StaticClass(), Actors);

		if (Actors.Num() > 0)
		{
			CurrentSection = Cast<ASTMissionSection>(Actors[UKismetMathLibrary::RandomIntegerInRange(0, Actors.Num() - 1)]);
		}
	}

	if (CurrentSection && CurrentSection->PlayerStarts.Num() > 0)
	{
		return CurrentSection->PlayerStarts[UKismetMathLibrary::RandomIntegerInRange(0, CurrentSection->PlayerStarts.Num() - 1)];
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}

void ASTGameMode_Roguelite::BeginPlay()
{
	Super::BeginPlay();

	/*
		Timer For Elite Boss
	*/
	if (CurrentSection)
	{
		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, CurrentSection, &ASTMissionSection::SpawnEliteBoss,
			SectionTimeLimit, false);
	}


	/*
		Timer For Final Boss (Teleport to Boss Map)
	*/

}

bool ASTGameMode_Roguelite::TeleportToNextSection()
{
	if (CachedSections.IsValidIndex(CurrentSectionIdx + 1))
	{
		TeleportPlayersToPlayerStarts(CachedSections[CurrentSectionIdx + 1]->PlayerStarts);
		return true;
	}

	return false;
}

void ASTGameMode_Roguelite::TeleportPlayersToPlayerStarts(const TArray<APlayerStart*>& inPlayerStarts)
{
	int playerCount = GetNumPlayers();

	if (inPlayerStarts.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("ASTGameMode_MissionBase : Failed to get PlayerStarts from Mission Section!!!"));
		return;
	}

	bool bShouldBindFunc = true;
	for (int i = 0; i < inPlayerStarts.Num(); i++)
	{
		ASTPlayerCharacter* player = Cast< ASTPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, i));
		if (player)
		{
			player->TeleportWithDissolve(inPlayerStarts[i]->GetTransform());

			if (bShouldBindFunc)
			{
				bShouldBindFunc = false;

				player->Delegate_TeleportEnded.RemoveAll(this);
				player->Delegate_TeleportEnded.AddDynamic(this, &ASTGameMode_Roguelite::OnTeleportEnded);
			}
		}
	}

	if (inPlayerStarts.Num() < playerCount)
	{
		// numOfPlayerStart : 2 && numOfPlayer : 4
		// -> startIdx = 4 - 2 = 2
		int startIdx = playerCount - inPlayerStarts.Num();
		for (int idx = startIdx; idx < playerCount; idx++)
		{
			ASTPlayerCharacter* player = Cast< ASTPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, idx));
			if (player)
			{
				player->TeleportWithDissolve(inPlayerStarts.Last()->GetTransform() + FTransform(FVector(100.f * idx, 0, 0)));
				// player->SetActorTransform(inPlayerStarts.Last()->GetTransform() + FTransform(FVector(100.f * idx, 0, 0)));
			}
		}
	}
}

bool ASTGameMode_Roguelite::ActivateNextSection()
{
	if (CachedSections.Num() > 0) 
	{
		CurrentSectionIdx++;

		if (CachedSections.IsValidIndex(CurrentSectionIdx))
		{
			CurrentSection = CachedSections[CurrentSectionIdx];

			ASTGameState* gameState = Cast<ASTGameState>(UGameplayStatics::GetGameState(this));
			if (CurrentSection && gameState)
			{
				CurrentSection->RegisterRandomMission();
				gameState->ActivateMission(CurrentSection);
				return true;
			}
		}
	}

	return false;
}

void ASTGameMode_Roguelite::OnTeleportEnded()
{
	// Activate Next Section && Set Timer for EliteBoss
	ActivateNextSection();

	if (CurrentSection)
	{
		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, CurrentSection, &ASTMissionSection::SpawnEliteBoss,
			SectionTimeLimit, false);
	}
}

void ASTGameMode_Roguelite::OnMissionCleared()
{

}

void ASTGameMode_Roguelite::EliteBossSpawned(ASTEliteBase* InElite)
{
	ASTGameState* gameState = Cast<ASTGameState>(UGameplayStatics::GetGameState(this));
	if (gameState)
	{
		gameState->OnEventOccur(FSTGameplayTags::Get().Event_EliteBossSpawned);
	}

	if (InElite)
	{
		InElite->Delegate_OnEnemyDied.AddDynamic(this, &ASTGameMode_Roguelite::OnEliteBossCleared);
	}
}

void ASTGameMode_Roguelite::OnEliteBossCleared(AActor* DiedEnemy)
{
	ASTGameState* gameState = Cast<ASTGameState>(UGameplayStatics::GetGameState(this));
	if (gameState)
	{
		gameState->OnEventOccur(FSTGameplayTags::Get().Event_EliteBossCleared);
		gameState->OnEventOccur(FSTGameplayTags::Get().Event_WaitForTeleport);
	}

	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle, [&]()
		{
			OnEliteBossClearCallback();
		}, 10.f, false);
}

void ASTGameMode_Roguelite::OnEliteBossClearCallback()
{
	// Teleport All Players to Next Section
	bool bIsTeleportSuccess = TeleportToNextSection();

	if (bIsTeleportSuccess)
	{
		// Despawn All Section Enemys
		if (CurrentSection)
		{
			CurrentSection->MissionDeactivated();
			CurrentSection->DespawnAllEnemys();
		}

		// Activate Next Section at ASTGameMode_Reguelite::OnTeleportEnded ! ! !
	}
	// Boss Map
	else
	{
		GetWorld()->ServerTravel(NextMapName.ToString());
	}
}
