// Fill out your copyright notice in the Description page of Project Settings.


#include "STAssetManager.h"
#include "STGameplayTags.h"

USTAssetManager& USTAssetManager::Get()
{
	check(GEngine);
	USTAssetManager* assetManager = Cast<USTAssetManager>(GEngine->AssetManager);
	return *assetManager;
}

void USTAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FSTGameplayTags::InitializeNativeGameplayTags();
}
