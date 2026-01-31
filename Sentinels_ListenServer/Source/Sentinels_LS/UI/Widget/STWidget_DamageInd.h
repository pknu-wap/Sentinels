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

protected:
	virtual void NativeConstruct() override;

public:
	void ClearDamageInd();

	void SetDamage(float Damage);
	void SetDamageTextColor(FSlateColor& InColor);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayCustomAnimation();

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UTextBlock* TB_Damage_0;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UTextBlock* TB_Damage_1;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UTextBlock* TB_Damage_2;

	UPROPERTY()
	TArray<UTextBlock*> TB_Damage;

	UPROPERTY(BlueprintReadWrite)
	int CurrentIdx = 0;
};
