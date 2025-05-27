// Fill out your copyright notice in the Description page of Project Settings.


#include "STGameplayTags.h"
#include "GameplayTagsManager.h"

FSTGameplayTags FSTGameplayTags::GameplayTags;

void FSTGameplayTags::InitializeNativeGameplayTags()
{
	/*
		Widget
	*/
	GameplayTags.Widget_OutGame_MainMenu = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Widget.OutGame.Mainmenu"),
		FString("Loadout")
	);

	GameplayTags.Widget_OutGame_System = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Widget.OutGame.System"),
		FString("Character Select")
	);

	GameplayTags.Widget_Lobby_Loadout = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Widget.Lobby.Loadout"),
		FString("Loadout")
	);

	GameplayTags.Widget_Lobby_WeaponSelect = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Widget.Lobby.WeaponSelect"),
		FString("Weapon Select")
	);

	GameplayTags.Widget_Lobby_Customize = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Widget.Lobby.Customize"),
		FString("Customize")
	);

	GameplayTags.Widget_Lobby_LevelSelect = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Widget.Lobby.LevelSelect"),
		FString("Level Select")
	);

	GameplayTags.Widget_InGame = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Widget.InGame"),
		FString("InGame")
	);

	GameplayTags.Widget_InGame_Map = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Widget.InGame.Map"),
		FString("Map")
	);

	GameplayTags.Widget_InGame_Minimap = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Widget.InGame.Minimap"),
		FString("Minimap")
	);

	GameplayTags.Widget_InGame_PlayerSkill = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Widget.InGame.PlayerSkill"),
		FString("Player Skill")
	);

	GameplayTags.Widget_InGame_PlayerState = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Widget.InGame.PlayerState"),
		FString("Player State")
	);

	GameplayTags.Widget_InGame_Quest = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Widget.InGame.Quest"),
		FString("Quest")
	);

	GameplayTags.Widget_InGame_Inventory = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Widget.InGame.Inventory"),
		FString("Character Select")
	);

	GameplayTags.Widget_InGame_TeamInfo = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Widget.InGame.TeamInfo"),
		FString("Team Info")
	);

	/*	
		Character
	*/
	GameplayTags.Character_State_Stunned = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Stunned"),
		FString("Character is Stunned")
	);

	GameplayTags.Character_State_Bleed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Bleed"),
		FString("Character State Bleed")
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

	GameplayTags.Character_Player_State_Lifting = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.Player.State.Lifting"),
		FString("Player is Lifting Anything"));

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

	GameplayTags.SubMission_Explosion = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("SubMission.Explosion"),
		FString("Explosion")
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

	GameplayTags.SpawnPoint_Hostage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("SpawnPoint.Hostage"),
		FString("SpawnPoint Hostage")
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

	GameplayTags.Level_MilitaryAirport = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Level.MilitaryAirport"),
		FString("MilitaryAirport")
	);

	GameplayTags.Level_MilitaryAirport = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Level.MilitaryAirport"),
		FString("MilitaryAirport")
	);

	GameplayTags.Level_EX2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Level.EX2"),
		FString("EX2")
	);

	/*
		Events
	*/
	GameplayTags.Level_MilitaryAirport_Event_Interact_ControlTower = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Level.MilitaryAirport.Event.Interact.ControlTower"),
		FString("MilitaryAirport Event : Interact Success With ControlTower")
	);

	GameplayTags.Level_MilitaryAirport_Event_Destroy_Vehicle = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Level.MilitaryAirport.Event.Destroy.Vehicle"),
		FString("MilitaryAirport Event : Destroy Vehicle")
	);

	GameplayTags.Level_MilitaryAirport_Event_Destroy_ControlTower = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Level.MilitaryAirport.Event.Destroy.ControlTower"),
		FString("MilitaryAirport Event : Destroy ControlTower")
	);
}
