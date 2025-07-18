// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Crunch/BTT_Crunch_DashAttack.h"
#include "STCrunchAnimInstance.h"
#include "Character/Enemy/Crunch/STCrunch.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTT_Crunch_DashAttack::UBTT_Crunch_DashAttack()
{
	ForceInstancing(true);
	NodeName = FString("Crunch Start DashAttack");
}

EBTNodeResult::Type UBTT_Crunch_DashAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* blackBoard = OwnerComp.GetBlackboardComponent();
	if (blackBoard)
	{
		AActor* target = Cast<AActor>(blackBoard->GetValueAsObject(FName("Target")));
		if (!target)
			return EBTNodeResult::Failed;


		ASTCrunch* Crunch = Cast<ASTCrunch>(OwnerComp.GetAIOwner()->GetPawn());
		if (!Crunch)
			return EBTNodeResult::Failed;

		// Set Dash Destination
		blackBoard->SetValueAsVector(FName("Dash_Destination"), target->GetActorLocation());

		// Set WarpTarget if using motion warp
		// Crunch->SetDashWarpTarget(FName("WarpTarget"), FTransform(target->GetActorLocation()));

		// Stop Movement && Update Animation
		Crunch->StartDash();
	}

    return EBTNodeResult::Succeeded;
}
