// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STWidget_Portal.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API USTWidget_Portal : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void VoteCountUpdated(int inCount);

protected:
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadWrite)
	class UTextBlock* TB_VoteCount;
};
