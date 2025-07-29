// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/STCharacterAnimInstanceBase.h"
#include "STHelixAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API USTHelixAnimInstance : public USTCharacterAnimInstanceBase
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation();
	
protected:
	UFUNCTION()
	void AnimNotify_Helix_Special_01();

	UFUNCTION()
	void AnimNotify_Helix_Special_02();

	UFUNCTION()
	void AnimNotify_Helix_Special_03();

	UFUNCTION()
	void AnimNotify_Helix_Cast();

private:
	UPROPERTY()
	class ASTHelix* helix;
};
