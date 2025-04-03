// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/STCharacterAnimInstanceBase.h"
#include "STPlayerAnimInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FStartCheckNextInput);
DECLARE_MULTICAST_DELEGATE(FCheckNextAttack);

UCLASS()
class SENTINELS_LS_API USTPlayerAnimInstance : public USTCharacterAnimInstanceBase
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void AnimNotify_StartCheckNextInput();

	UFUNCTION()
	void AnimNotify_CheckNextAttack();

public:
	FStartCheckNextInput Delegate_StartCheckNextInput;
	FCheckNextAttack Delegate_CheckNextAttack;

};
