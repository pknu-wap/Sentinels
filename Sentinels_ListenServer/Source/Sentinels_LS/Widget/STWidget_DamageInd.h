// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STWidget_DamageInd.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API USTWidget_DamageInd : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetDamage(float Damage);
	void SetTextColor(FSlateColor& InColor);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayCustomAnimation();

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_Damage;
};
