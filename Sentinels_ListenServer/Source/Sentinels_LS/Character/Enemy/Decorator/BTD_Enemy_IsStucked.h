// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTD_Enemy_IsStucked.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API UBTD_Enemy_IsStucked : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTD_Enemy_IsStucked(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
    virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;
    virtual void OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult) override;
    virtual void OnNodeProcessed(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult);
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
    /** 얼마나 안 움직였을 때 Stuck으로 간주할지 (cm) */
    UPROPERTY(EditAnywhere, Category = "Stuck Detection")
    float MinMovementThreshold;

    /** 몇 초 이상 멈춰있으면 Stuck */
    UPROPERTY(EditAnywhere, Category = "Stuck Detection")
    float StuckTimeThreshold;

private:
    UPROPERTY()
    FVector LastLocation;

    UPROPERTY()
    float AccumulatedTime = 0.f;

    UPROPERTY()
    bool bIsStucked = false;
   
};
