// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Decorator/BTD_Enemy_IsStucked.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_Enemy_IsStucked::UBTD_Enemy_IsStucked(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "IsStucked";

	bNotifyTick = true;
	ForceInstancing(true);

    bNotifyActivation = true;
    bNotifyDeactivation = true;
    bNotifyProcessed = true;

	MinMovementThreshold = 5.f;
	StuckTimeThreshold = 2.0f;
}

void UBTD_Enemy_IsStucked::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	Super::OnNodeActivation(SearchData);

    bIsStucked = false;
    // AccumulatedTime = 0.f;
}

void UBTD_Enemy_IsStucked::OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult)
{
    Super::OnNodeDeactivation(SearchData, NodeResult);

    bIsStucked = false;
    // AccumulatedTime = 0.f;
}

void UBTD_Enemy_IsStucked::OnNodeProcessed(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult)
{
    Super::OnNodeProcessed(SearchData, NodeResult);

    bIsStucked = false;
    // AccumulatedTime = 0.f;
}

void UBTD_Enemy_IsStucked::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* Controller = OwnerComp.GetAIOwner();
    if (!Controller || !Controller->GetPawn()) return;

    FVector CurrentLocation = Controller->GetPawn()->GetActorLocation();
    float DistanceMoved = FVector::Dist2D(CurrentLocation, LastLocation);

    if (DistanceMoved < MinMovementThreshold)
    {
        AccumulatedTime += DeltaSeconds;
    }
    else
    {
        AccumulatedTime = 0.f;
        LastLocation = CurrentLocation;
    }

    if (AccumulatedTime >= StuckTimeThreshold)
    {
        AccumulatedTime = 0.f;
        bIsStucked = true;

        UBlackboardComponent* blackBoard = OwnerComp.GetBlackboardComponent();
        if (blackBoard)
        {
            blackBoard->SetValueAsBool(FName("IsStucked"), true);
        }
    }
}

bool UBTD_Enemy_IsStucked::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    return !bIsStucked;
}
