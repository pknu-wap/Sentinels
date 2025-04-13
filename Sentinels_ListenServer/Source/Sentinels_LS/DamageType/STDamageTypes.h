// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "STDamageTypes.generated.h"

UCLASS()
class SENTINELS_LS_API USTBaseDamageType : public UDamageType
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StunnedTime = 1.f;
};
