// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API USTUserWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateUI();
	
};
