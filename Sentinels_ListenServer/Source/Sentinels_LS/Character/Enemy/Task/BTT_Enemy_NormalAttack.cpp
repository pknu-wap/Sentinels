// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Task/BTT_Enemy_NormalAttack.h"
#include "Character/Enemy/STEnemyBase.h"
#include "Character/STCharacterAnimInstanceBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTT_Enemy_NormalAttack::UBTT_Enemy_NormalAttack()
{
	bNotifyTick = true;
	ForceInstancing(true);
	NodeName = FString("Enemy NormalAttack");
}

EBTNodeResult::Type UBTT_Enemy_NormalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	Enemy = Cast<ASTEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;
	UAnimInstance* AnimInst = Enemy->GetMesh()->GetAnimInstance();
	if (!AnimInst) return EBTNodeResult::Failed;

	// Clean Task Values
	IsMontageIntrrupted = false;
	IsMontagePlaying = true;
	CleanUp();

	// Binding Function
	AnimInst->OnMontageEnded.AddDynamic(this, &UBTT_Enemy_NormalAttack::OnIntrruptedCallback);

	// Play Montage
	Enemy->ActivateNormalAttack_Server();

	return EBTNodeResult::InProgress;
}

void UBTT_Enemy_NormalAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsMontagePlaying || IsMontageIntrrupted)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTT_Enemy_NormalAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTT_Enemy_NormalAttack::CleanUp()
{
	UAnimInstance* AnimInst = Enemy->GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->OnMontageEnded.Clear();
	}
}

void UBTT_Enemy_NormalAttack::OnIntrruptedCallback(UAnimMontage* Montage, bool Interrupted)
{
	if (Interrupted)
	{
		// Should Check Montage Type ( Don't Stop Task When Interrupted by AttackMontage! )
		
	}
	else
	{
		IsMontagePlaying = false;
	}
}
