// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Crunch/BTT_Crunch_DashAttack_Stop.h"
#include "STCrunchAnimInstance.h"
#include "Character/Enemy/Crunch/STCrunch.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTT_Crunch_DashAttack_Stop::UBTT_Crunch_DashAttack_Stop()
{
	ForceInstancing(true);
	NodeName = FString("Crunch Stop DashAttack");
}

EBTNodeResult::Type UBTT_Crunch_DashAttack_Stop::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* blackBoard = OwnerComp.GetBlackboardComponent();
	if (blackBoard)
	{
		ASTCrunch* Crunch = Cast<ASTCrunch>(OwnerComp.GetAIOwner()->GetPawn());
		if (!Crunch)
			return EBTNodeResult::Failed;

		Crunch->EndDash();
	}

	return EBTNodeResult::Succeeded;
}
