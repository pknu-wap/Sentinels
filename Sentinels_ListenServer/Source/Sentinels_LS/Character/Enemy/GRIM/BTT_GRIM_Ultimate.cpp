// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/GRIM/BTT_GRIM_Ultimate.h"
#include "Character/Enemy/STEnemyBase.h"
#include "Character/Enemy/GRIM/STGRIM.h"
#include "Character/STCharacterAnimInstanceBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTT_GRIM_Ultimate::UBTT_GRIM_Ultimate()
{
	bNotifyTick = true;
	ForceInstancing(true);
	NodeName = FString("GRIM Ultimate");
}

EBTNodeResult::Type UBTT_GRIM_Ultimate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	GRIM = Cast<ASTGRIM>(OwnerComp.GetAIOwner()->GetPawn());
	if (!GRIM) return EBTNodeResult::Failed;
	UAnimInstance* AnimInst = GRIM->GetMesh()->GetAnimInstance();
	if (!AnimInst) return EBTNodeResult::Failed;

	// Clean Task Values
	IsMontageIntrrupted = false;
	IsMontagePlaying = true;
	CleanUp();

	// Binding Function
	AnimInst->OnMontageEnded.AddDynamic(this, &UBTT_GRIM_Ultimate::OnIntrruptedCallback);

	// Play Montage
	GRIM->ActivateUltimate_Server();

	return EBTNodeResult::InProgress;
}

void UBTT_GRIM_Ultimate::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsMontagePlaying)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else if (IsMontageIntrrupted)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

void UBTT_GRIM_Ultimate::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTT_GRIM_Ultimate::CleanUp()
{
	UAnimInstance* AnimInst = GRIM->GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->OnMontageEnded.Clear();
	}
}

void UBTT_GRIM_Ultimate::OnIntrruptedCallback(UAnimMontage* Montage, bool Interrupted)
{
	if (Interrupted)
	{
		// Should Check Montage Type ( Don't Stop Task When Interrupted by AttackMontage! )
		UAnimMontage* CurrentMontage = GRIM->GetCurrentMontage();
		if (GRIM && CurrentMontage != GRIM->Montage_Ultimate)
		{
			IsMontageIntrrupted = true;
		}
	}
	else
	{
		IsMontagePlaying = false;
	}
}
