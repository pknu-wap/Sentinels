// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/STCharacterAnimInstanceBase.h"
#include "STCrunchAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API USTCrunchAnimInstance : public USTCharacterAnimInstanceBase
{
	GENERATED_BODY()
	
public:
	void StartDash() { bShouldDash = true; }
	void EndDash() { bShouldDash = false; }

protected:
	UPROPERTY(BlueprintReadWrite)
	bool bShouldDash = false;
};
