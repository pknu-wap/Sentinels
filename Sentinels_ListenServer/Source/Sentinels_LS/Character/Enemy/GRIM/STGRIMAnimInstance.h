// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/STPlayerAnimInstance.h"
#include "STGRIMAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FGrimPrimaryFire)

UCLASS()
class SENTINELS_LS_API USTGRIMAnimInstance : public USTPlayerAnimInstance
{
	GENERATED_BODY()
	
	UFUNCTION()
	void AnimNotify_GrimPrimaryFire();

public:
	FGrimPrimaryFire Delegate_GrimPrimaryFire;
};
