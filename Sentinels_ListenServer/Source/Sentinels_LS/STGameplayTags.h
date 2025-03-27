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
	FGameplayTag Widget_Lobby_Ready;
	FGameplayTag Widget_Lobby_CharacterSelect;
	FGameplayTag Widget_Lobby_SkillSelect;

	//////////  Mission  /////////
	FGameplayTag Mission_Domination;
	FGameplayTag Mission_Eliminate_EliteMonster;
	FGameplayTag Mission_RescueHostage;
	FGameplayTag Mission_EscortObject;
	FGameplayTag Mission_DestroyObjects;
	FGameplayTag Mission_Collect_QuestItems;
	FGameplayTag Mission_RepairRift;
	FGameplayTag Mission_FinalDefense;


private:
	static FSTGameplayTags GameplayTags;
};