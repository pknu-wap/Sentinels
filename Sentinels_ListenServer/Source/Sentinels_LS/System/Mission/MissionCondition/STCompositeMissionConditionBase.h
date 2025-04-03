// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/NetworkObject.h"
#include "STMissionConditionBase.h"
#include "STCompositeMissionConditionBase.generated.h"

UENUM()
enum class EConditionCombineType : uint8
{
    AND,
    OR
};

UCLASS()
class SENTINELS_LS_API USTCompositeMissionConditionBase : public USTMissionConditionBase
{
	GENERATED_BODY()

protected:
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
    virtual void PostInitProperties() override;

    virtual bool IsSatisfied();

public:
    UPROPERTY(Replicated)
    TArray<USTMissionConditionBase*> SubConditions;

    UPROPERTY(EditAnywhere)
    TArray<TSubclassOf<USTMissionConditionBase>> SubConditionClasses;

    UPROPERTY(EditAnywhere)
    EConditionCombineType CombineType;
};
