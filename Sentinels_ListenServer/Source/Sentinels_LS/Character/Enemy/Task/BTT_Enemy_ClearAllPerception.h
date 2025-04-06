// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Enemy_ClearAllPerception.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API UBTT_Enemy_ClearAllPerception : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_Enemy_ClearAllPerception();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere)
	float ClearDelay = 0.5f;
};
