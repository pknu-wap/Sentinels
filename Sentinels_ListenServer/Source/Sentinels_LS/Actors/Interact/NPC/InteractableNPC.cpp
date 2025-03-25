// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interact/NPC/InteractableNPC.h"
#include "Kismet/GameplayStatics.h"
#include "System/STGameState.h"
#include "System/Mission/STMissionBase.h"

void AInteractableNPC::Interact()
{
	ASTGameState* GameState = Cast<ASTGameState>(UGameplayStatics::GetGameState(this));
	if (GameState)
	{
		USTMissionBase* mission = GameState->GetMission(FSTGameplayTags::Get().Mission_RescueHostage);
		if (mission)
		{
			mission->UpdateRescueHostageInfo_Server(NPCID);
		}

		// GameState->UpdateRescueHostageInfo_Server(NPCID);
	}
}