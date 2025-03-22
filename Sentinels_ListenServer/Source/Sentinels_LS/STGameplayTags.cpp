// Fill out your copyright notice in the Description page of Project Settings.


#include "STGameplayTags.h"
#include "GameplayTagsManager.h"

FSTGameplayTags FSTGameplayTags::GameplayTags;

void FSTGameplayTags::InitializeNativeGameplayTags()
{
	// Sample
	/*
	GameplayTags.Character_State_Attacking = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.State.Attacking"),
		FString("Attacking")
	);
	*/
}
