// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/STCharacterAnimInstanceBase.h"
#include "STGRIMAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FGrimUltimateFire)

UCLASS()
class SENTINELS_LS_API USTGRIMAnimInstance : public USTCharacterAnimInstanceBase
{
	GENERATED_BODY()
	
	UFUNCTION()
	void AnimNotify_GrimUltimateFire();

public:
	FGrimUltimateFire Delegate_GrimUlitmateFire;
};
