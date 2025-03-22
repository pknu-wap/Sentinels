// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_UpdateTargetLocation.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API UBTS_UpdateTargetLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTS_UpdateTargetLocation();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere)
	float MaxDistance = 3000.f;
};
