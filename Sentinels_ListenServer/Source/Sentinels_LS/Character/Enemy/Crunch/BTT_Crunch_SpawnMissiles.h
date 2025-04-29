// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Crunch_SpawnMissiles.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API UBTT_Crunch_SpawnMissiles : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_Crunch_SpawnMissiles();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
