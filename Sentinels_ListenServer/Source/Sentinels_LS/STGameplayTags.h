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
	FGameplayTag Character_State_Jump;
	FGameplayTag Character_State_Attack;
	FGameplayTag Character_State_Skill;
	FGameplayTag Character_State_Step;
	FGameplayTag Character_State_Stunned;
	FGameplayTag Character_State_Bleed;
	FGameplayTag Character_State_Invincible;
	FGameplayTag Character_State_Reflect;
	FGameplayTag Character_Player_State_RepairRift;
	FGameplayTag Character_Player_State_RescueHostage;
	FGameplayTag Character_Player_State_Lifting;

	//////////  Widget   /////////
	FGameplayTag Widget_OutGame_MainMenu;
	FGameplayTag Widget_OutGame_System;
	FGameplayTag Widget_Lobby_Loadout;
	FGameplayTag Widget_Lobby_WeaponSelect;
	FGameplayTag Widget_Lobby_LevelSelect;
	FGameplayTag Widget_Lobby_Customize;
	FGameplayTag Widget_InGame;
	FGameplayTag Widget_InGame_Map;
	FGameplayTag Widget_InGame_Minimap;
	FGameplayTag Widget_InGame_PlayerSkill;
	FGameplayTag Widget_InGame_PlayerState;
	FGameplayTag Widget_InGame_Quest;
	FGameplayTag Widget_InGame_Inventory;
	FGameplayTag Widget_InGame_TeamInfo;
	FGameplayTag Widget_InGame_Interactive;


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

	FGameplayTag SubMission_Explosion;


	//////////  Section  /////////
	FGameplayTag Section_SelectMissionOrBoss;


	/////////  SpawnPoint  /////////
	FGameplayTag SpawnPoint_DefenseCore;
	FGameplayTag SpawnPoint_DominationPoint;
	FGameplayTag SpawnPoint_Hostage;
	FGameplayTag SpawnPoint_Rift;
	FGameplayTag SpawnPoint_Spawner;
	FGameplayTag SpawnPoint_DestructibleObject;

	/////////  Level  /////////
	FGameplayTag Level_Lobby;
	FGameplayTag Level_Boss;
	FGameplayTag Level_MilitaryAirport;

	/////////  Event  /////////
	FGameplayTag Level_MilitaryAirport_Event_Interact_ControlTower;
	FGameplayTag Level_MilitaryAirport_Event_Destroy_Vehicle;
	FGameplayTag Level_MilitaryAirport_Event_Destroy_ControlTower;

private:
	static FSTGameplayTags GameplayTags;
};