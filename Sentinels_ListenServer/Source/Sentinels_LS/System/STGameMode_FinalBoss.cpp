// Fill out your copyright notice in the Description page of Project Settings.


#include "System/STGameMode_FinalBoss.h"
#include "System/STGameState_FinalBoss.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Enemy/STEliteBase.h"
#include "SubSystem/STSessionSubSystem.h"
#include "Components/InventoryComponent.h"
#include "Components/STPlayerStatusComponent.h"
#include "Player/STPlayerState.h"
#include "Player/STPlayerCharacter.h"

void ASTGameMode_FinalBoss::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	ASTEliteBase* finalBoss = Cast<ASTEliteBase>(UGameplayStatics::GetActorOfClass(this, ASTEliteBase::StaticClass()));
	if (finalBoss)
	{
		finalBoss->Delegate_OnEnemyDied.AddDynamic(this, &ASTGameMode_FinalBoss::BossClearCallback);
	}
}

void ASTGameMode_FinalBoss::BossClearCallback(AActor* ClearedBoss)
{
	if (ASTGameState_FinalBoss* GS = GetGameState<ASTGameState_FinalBoss>())
	{
		GS->OnBossCleared_Multicast();

		// Clear Player's Inventory & Status
		// Dissolve Players
		for (APlayerState* PS : GS->PlayerArray)
		{
			if (ASTPlayerState* STPS = Cast<ASTPlayerState>(PS))
			{
				STPS->ClearAllState();
			}

			if (ASTPlayerCharacter* PlayerCharacter = Cast<ASTPlayerCharacter>(PS->GetPawn()))
			{
				PlayerCharacter->DissolveStart_Multicast();
				// PlayerCharacter->TeleportWithDissolve()
			}
		}

		// Return To Lobby
		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, this, &ASTGameMode_FinalBoss::ServerTravelToLobby, 7.5f, false);
	}
}

void ASTGameMode_FinalBoss::ServerTravelToLobby()
{
	GetWorld()->ServerTravel(FName("LV_SessionLobby").ToString());
}
