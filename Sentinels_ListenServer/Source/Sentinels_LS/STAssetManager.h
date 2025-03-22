// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "STAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API USTAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	static USTAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
