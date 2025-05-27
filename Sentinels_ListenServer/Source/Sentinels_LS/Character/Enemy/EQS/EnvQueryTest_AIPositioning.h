// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_AIPositioning.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API UEnvQueryTest_AIPositioning : public UEnvQueryTest
{
	GENERATED_BODY()
	
public:
	UEnvQueryTest_AIPositioning();

    virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

};
