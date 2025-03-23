// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interact/Rift/InteractableRift.h"
#include "Kismet/GameplayStatics.h"
#include "System/STGameState.h"
#include "STGameplayTags.h"
#include "System/Mission/STMissionBase.h"

void AInteractableRift::Interact()
{
	ASTGameState* GameState = Cast<ASTGameState>(UGameplayStatics::GetGameState(this));
	if (GameState)
	{
		USTMissionBase* mission = GameState->GetMission(FSTGameplayTags::Get().Mission_RepairRift);
		if (mission)
		{
			mission->UpdateRepairRiftInfo_Server(RiftID);
		}

		// GameState->UpdateRepairRiftInfo_Server(RiftID);
	}
}
