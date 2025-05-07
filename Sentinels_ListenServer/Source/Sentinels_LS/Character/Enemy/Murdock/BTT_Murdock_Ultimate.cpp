// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Murdock/BTT_Murdock_Ultimate.h"
#include "Character/Enemy/STEnemyBase.h"
#include "Character/Enemy/Murdock/STMurdock.h"
#include "Character/STCharacterAnimInstanceBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTT_Murdock_Ultimate::UBTT_Murdock_Ultimate()
{
	bNotifyTick = true;
	ForceInstancing(true);
	NodeName = FString("Murdock Ultimate");
}

EBTNodeResult::Type UBTT_Murdock_Ultimate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	Murdock = Cast<ASTMurdock>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Murdock) return EBTNodeResult::Failed;
	UAnimInstance* AnimInst = Murdock->GetMesh()->GetAnimInstance();
	if (!AnimInst) return EBTNodeResult::Failed;

	// Clean Task Values
	IsMontageIntrrupted = false;
	IsMontagePlaying = true;
	CleanUp();

	// Binding Function
	AnimInst->OnMontageEnded.AddDynamic(this, &UBTT_Murdock_Ultimate::OnIntrruptedCallback);

	// Play Montage
	Murdock->ActivateUltimate_Server();

	return EBTNodeResult::InProgress;
}

void UBTT_Murdock_Ultimate::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

void UBTT_Murdock_Ultimate::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTT_Murdock_Ultimate::CleanUp()
{
	UAnimInstance* AnimInst = Murdock->GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->OnMontageEnded.Clear();
	}
}

void UBTT_Murdock_Ultimate::OnIntrruptedCallback(UAnimMontage* Montage, bool Interrupted)
{
	if (Interrupted)
	{
		// Should Check Montage Type ( Don't Stop Task When Interrupted by AttackMontage! )
		UAnimMontage* CurrentMontage = Murdock->GetCurrentMontage();
		if (Murdock && CurrentMontage != Murdock->Montage_Ultimate)
		{
			IsMontageIntrrupted = true;
		}
	}
	else
	{
		IsMontagePlaying = false;
	}
}
