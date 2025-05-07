// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/STEventSubsystem.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

USTEventSubsystem::USTEventSubsystem()
{
}

void USTEventSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

    FGameplayTagContainer AllTags;
    UGameplayTagsManager::Get().RequestAllGameplayTags(AllTags, false);

    for (auto& Tag : AllTags)
    {

    }
}
