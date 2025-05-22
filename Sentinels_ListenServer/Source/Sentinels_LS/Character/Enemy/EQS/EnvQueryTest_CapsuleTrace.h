// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_CapsuleTrace.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API UEnvQueryTest_CapsuleTrace : public UEnvQueryTest
{
	GENERATED_BODY()
	
public:
    UEnvQueryTest_CapsuleTrace();

    virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

private:
    UPROPERTY(EditAnywhere, Category = "Capsule Trace")
    FAIDataProviderFloatValue CapsuleRadius;
};
