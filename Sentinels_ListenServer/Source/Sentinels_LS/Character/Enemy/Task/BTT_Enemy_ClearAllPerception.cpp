// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Task/BTT_Enemy_ClearAllPerception.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"

UBTT_Enemy_ClearAllPerception::UBTT_Enemy_ClearAllPerception()
{
	bNotifyTick = false;
	NodeName = FString("Enemy Clear All Perception");
}

EBTNodeResult::Type UBTT_Enemy_ClearAllPerception::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		UAIPerceptionComponent* Perception = AIController->GetPerceptionComponent();
		if (Perception)
		{
			FTimerHandle handle;
			AIController->GetWorldTimerManager().SetTimer(handle, 
				[Perception]() {Perception->ForgetAll(); },
				ClearDelay, false);
		}
	}

	return EBTNodeResult::Succeeded;
}
