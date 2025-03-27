// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionCondition/RescueHostage/STRescueHostageCondition.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/SpawnPoint/SpawnPointBase.h"
#include "Actors/Interact/NPC/InteractableNPC.h"
#include "Sentinels_LS.h"
#include "STGameplayTags.h"
#include "System/STGameState.h"
#include "System/Mission/STMissionBase.h"

void USTRescueHostageCondition::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USTRescueHostageCondition, HostageInfos)
}

bool USTRescueHostageCondition::IsSatisfied()
{
	for (int i = 0; i < HostageInfos.Num(); i++)
	{
		if (HostageInfos[i].bIsRescued == false)
		{
			return false;
		}
	}

	return true;
}

void USTRescueHostageCondition::MissionActivated()
{/*
		Set Mission
	*/
	CurrentMissionTime = 0;

	/*
		Spawn NPC On Random Point (Get All NPC Spawn Points From World)
	*/
	FVector SpawnLocation; FRotator SpawnRotation;

	TArray<ASpawnPointBase*> SpawnPoints;
	GetAllSpawnPointsWithTag(FSTGameplayTags::Get().SpawnPoint_NPC, SpawnPoints);

	if (SpawnPoints.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("USTMission_RepairRift::ActivateMission Failed to get SpawnPoints of NPC!"));
		return;
	}

	if (SpawnPoints.Num() < HostageInfos.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("USTMission_RepairRift::ActivateMission SpawnPoints Num is Smaller than Repair Rift Num."));
		return;
	}

	TSet<int> UsedSpawnPoints;
	for (int i = 0; i < HostageInfos.Num(); i++)
	{
		// Set Random Point From SpawnPoints

		int Rand = -1;
		while (true)
		{
			Rand = UKismetMathLibrary::RandomIntegerInRange(0, SpawnPoints.Num() - 1);
			if (!UsedSpawnPoints.Contains(Rand))
			{
				UsedSpawnPoints.Add(Rand);
				break;
			}
		}

		if (!SpawnPoints.IsValidIndex(Rand))
		{
			continue;
		}

		SpawnLocation = SpawnPoints[Rand]->GetActorLocation();
		SpawnRotation = SpawnPoints[Rand]->GetActorRotation();

		if (HostageInfos[i].SubClassOfNPC)
		{
			AInteractableNPC* npc = GetWorld()->SpawnActor<AInteractableNPC>(HostageInfos[i].SubClassOfNPC, SpawnLocation, SpawnRotation);
			if (npc)
			{
				npc->SetNPCID(HostageInfos[i].NPCID);
				npc->Delegate_MissionConditionUpdate.AddUObject(this, &USTRescueHostageCondition::ConditionUpdated);
			}
		}
	}


	/*
	*	Show Related Widget on Server & Clients
	*/
}

void USTRescueHostageCondition::MissionDeactivated(bool IsCleared)
{
}

void USTRescueHostageCondition::ConditionUpdated(int ObjectID, bool Success)
{
	for (int i = 0; i < HostageInfos.Num(); i++)
	{
		if (HostageInfos[i].NPCID == ObjectID)
		{
			HostageInfos[i].bIsRescued = true;
		}
	}

	if (IsSatisfied())
	{
		ASTGameState* GameState = Cast<ASTGameState>(GetWorld()->GetGameState());
		if (GameState)
		{
			USTMissionBase* Mission = GameState->GetMission(MissionTag);
			if (Mission)
			{
				Mission->CheckMissionClearable();
			}
		}
	}
}

void USTRescueHostageCondition::OnRep_HostageInfos()
{
	UE_LOG(LogTemp, Display, TEXT("USTRescueHostageCondition::OnRep_HostageInfos"));
}
