// Fill out your copyright notice in the Description page of Project Settings.


#include "STGameplayTags.h"
#include "GameplayTagsManager.h"

FSTGameplayTags FSTGameplayTags::GameplayTags;

void FSTGameplayTags::InitializeNativeGameplayTags()
{
	/*
		Widget
	*/

	GameplayTags.Widget_Lobby_Loadout = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Widget.Lobby.Loadout"),
		FString("Loadout")
	);

	GameplayTags.Widget_Lobby_CharacterSelect = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Widget.Lobby.CharacterSelect"),
		FString("Character Select")
	);

	GameplayTags.Widget_Lobby_LevelSelect = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Widget.Lobby.LevelSelect"),
		FString("Level Select")
	);

	/*	
		Character
	*/
	GameplayTags.Character_State_Stunned = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Stunned"),
		FString("Character is Stunned")
	);
	
	GameplayTags.Character_State_Jump = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Jump"),
		FString("Character State Jump")
	);

	GameplayTags.Character_State_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Attack"),
		FString("Character State Attack")
	);

	GameplayTags.Character_State_Skill = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Skill"),
		FString("Character State Skill")
	);

	GameplayTags.Character_State_Step = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Step"),
		FString("Character State Step")
	);

	GameplayTags.Character_Player_State_RepairRift = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.Player.State.RepairRift"),
		FString("Player is Reparing Rift")
	);

	GameplayTags.Character_Player_State_RescueHostage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.Player.State.RescueHostage"),
		FString("Player is Rescue Hostage")
	);

	/*
		Missions
	*/
	
	GameplayTags.Mission_Domination = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Mission.Domination"),
		FString("Domination")
	);

	GameplayTags.Mission_DefenseWave = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Mission.DefenseWave"),
		FString("DefenseWave")
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

	/*
		Spawn Points
	*/
	GameplayTags.SpawnPoint_DefenseCore = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("SpawnPoint.DefenseCore"),
		FString("SpawnPoint DefenseCore")
	);

	GameplayTags.SpawnPoint_DominationPoint = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("SpawnPoint.DominationPoint"),
		FString("SpawnPoint DominationPoint")
	);

	GameplayTags.SpawnPoint_NPC = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("SpawnPoint.NPC"),
		FString("SpawnPoint NPC")
	);

	GameplayTags.SpawnPoint_Rift = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("SpawnPoint.Rift"),
		FString("SpawnPoint Rift")
	);

	GameplayTags.SpawnPoint_Spawner = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("SpawnPoint.Spawner"),
		FString("SpawnPoint Spawner")
	);

	GameplayTags.SpawnPoint_DestructibleObject = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("SpawnPoint.DestructibleObject"),
		FString("SpawnPoint DestructibleObject")
	);

	/*
		Levels
	*/
	GameplayTags.Level_Lobby = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Level.Lobby"),
		FString("Lobby")
	);

	GameplayTags.Level_AbandonedFactory = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Level.AbandonedFactory"),
		FString("AbandonedFactory")
	);

	GameplayTags.Level_EX2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Level.EX2"),
		FString("EX2")
	);
}
