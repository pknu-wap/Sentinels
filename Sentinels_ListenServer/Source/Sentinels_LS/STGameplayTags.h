// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FSTGameplayTags
{

public:
	static const FSTGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	///////////  Tags  ///////////

	////////// Character /////////

	//////////  Widget   /////////

	//////////  Mission  /////////
	FGameplayTag Mission_Domination;
	FGameplayTag Mission_DefenseWave;
	FGameplayTag Mission_Eliminate_EliteMonster;
	FGameplayTag Mission_RescueHostage;
	FGameplayTag Mission_EscortObject;
	FGameplayTag Mission_DestroyObjects;
	FGameplayTag Mission_Collect_QuestItems;
	FGameplayTag Mission_RepairRift;
	FGameplayTag Mission_FinalDefense;

	/////////  SpawnPoint  /////////
	FGameplayTag SpawnPoint_DefenseCore;
	FGameplayTag SpawnPoint_DominationPoint;
	FGameplayTag SpawnPoint_NPC;
	FGameplayTag SpawnPoint_Rift;
	FGameplayTag SpawnPoint_Spawner;
	FGameplayTag SpawnPoint_DestructibleObject;

private:
	static FSTGameplayTags GameplayTags;
};