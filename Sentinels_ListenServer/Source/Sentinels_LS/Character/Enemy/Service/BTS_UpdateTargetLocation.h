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
	/*
		if greater than 0, Return to start Location when Out Fram Start Location !
	*/
	UPROPERTY(EditAnywhere)
	float MaxDistance_FromStartLocation = 0.f;
};
