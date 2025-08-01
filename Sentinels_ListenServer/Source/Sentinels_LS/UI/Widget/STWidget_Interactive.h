// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STWidget_Interactive.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API USTWidget_Interactive : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetInteractiveUI(const struct FItemStruct* ItemInfo);
	void ClearInteractiveUI();
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_Name;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_Description;
};
