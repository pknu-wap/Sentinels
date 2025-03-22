// Fill out your copyright notice in the Description page of Project Settings.


#include "STGameplayTags.h"
#include "GameplayTagsManager.h"

FSTGameplayTags FSTGameplayTags::GameplayTags;

void FSTGameplayTags::InitializeNativeGameplayTags()
{

	/*
		Missions
	*/

	GameplayTags.Mission_Domination = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Mission.Domination"),
		FString("Domination")
	);

	GameplayTags.Mission_Eliminate_EliteMonster = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Mission.Eliminate_EliteMonster"),
		FString("Eliminate EliteMonster")
	);

	GameplayTags.Mission_RescueHostage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Mission.RescueHostage"),
		FString("Rescue Hostage")
	);

	GameplayTags.Mission_EscortObject = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Mission.EscortObject"),
		FString("Escort Object")
	);

	GameplayTags.Mission_DestroyObjects = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Mission.DestroyObjects"),
		FString("Destroy Objects")
	);

	GameplayTags.Mission_Collect_QuestItems = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Mission.Collect_QuestItems"),
		FString("Collect QuestItems")
	);

	GameplayTags.Mission_RepairRift = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Mission.RepairRift"),
		FString("Repair Rift")
	);

	GameplayTags.Mission_FinalDefense = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Mission.FinalDefense"),
		FString("Final Defense Until Lift Activate")
	);
}
