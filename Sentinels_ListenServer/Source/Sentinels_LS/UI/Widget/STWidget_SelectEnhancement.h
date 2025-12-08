// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STWidget_SelectEnhancement.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API USTWidget_SelectEnhancement : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void ShowSelections(const TArray<FGameplayTag>& Selections);
};
