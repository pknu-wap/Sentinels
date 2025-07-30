// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Crunch_DashAttack.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API UBTT_Crunch_DashAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTT_Crunch_DashAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY()
	class USTCrunchAnimInstance* animInst_Crunch;
};
