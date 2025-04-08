// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/STEnemy_EntireMap_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Player/STPlayerCharacter.h"

void ASTEnemy_EntireMap_AIController::OnTargetDetected(AActor* actor, const FAIStimulus Stimulus)
{
	if (!IsValid(Blackboard))
	{
		UE_LOG(LogTemp, Warning, TEXT("ASTEnemyBase_AIController : Blackboard is not valid."));
		return;
	}

	if (actor == nullptr)
	{
		Blackboard->SetValueAsObject(BBKey_Target, nullptr);
	}
	else
	{
		Super::OnTargetDetected(actor, Stimulus);
	}
}
