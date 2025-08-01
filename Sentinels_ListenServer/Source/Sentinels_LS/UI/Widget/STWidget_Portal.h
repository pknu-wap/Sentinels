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
	int GetCurrentVoteCount() { return CurrentVoteCount; }
	// net id를 받아서 수정하도록
	// portal manager를 만들어야 함
	void SetCurrentVoteCount(int& InValue) { CurrentVoteCount = InValue; }

	class UTextBlock* GetTB_VoteCount() { return TB_VoteCount; }

	UFUNCTION(BlueprintImplementableEvent)
	void SetTB_VoteCount();

protected:
	UPROPERTY(meta = (BindWidget), EditAnywhere, BlueprintReadWrite)
	class UTextBlock* TB_VoteCount;

	UPROPERTY(BlueprintReadWrite)
	int CurrentVoteCount = 0;
};
