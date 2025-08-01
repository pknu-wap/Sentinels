// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Overlay.h"
#include "UI/Widget/STWidget_DamageInd.h"
#include "STWidget_EnemyMain_Screen.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API USTWidget_EnemyMain_Screen : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UOverlay* Overlay_DamageInd;

	UPROPERTY(BlueprintReadWrite)
	AActor* Owner;
	
};
