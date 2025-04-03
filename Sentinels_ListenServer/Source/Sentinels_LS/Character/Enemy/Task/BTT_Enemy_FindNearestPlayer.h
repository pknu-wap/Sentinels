// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Enemy_FindNearestPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API UBTT_Enemy_FindNearestPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_Enemy_FindNearestPlayer();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
