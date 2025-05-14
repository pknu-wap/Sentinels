// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Task/BTT_EliteEnemy_Attack.h"
#include "Character/Enemy/STEliteBase.h"
#include "Character/STCharacterAnimInstanceBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTT_EliteEnemy_Attack::UBTT_EliteEnemy_Attack()
{
	bNotifyTick = true;
	ForceInstancing(true);
	NodeName = FString("EliteEnemy Attack");
}

EBTNodeResult::Type UBTT_EliteEnemy_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	EliteEnemy = Cast<ASTEliteBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!EliteEnemy) return EBTNodeResult::Failed;
	UAnimInstance* AnimInst = EliteEnemy->GetMesh()->GetAnimInstance();
	if (!AnimInst) return EBTNodeResult::Failed;

	// Clean Task Values
	IsMontageIntrrupted = false;
	IsMontagePlaying = true;
	CleanUp();

	// Binding Function
	AnimInst->OnMontageEnded.AddDynamic(this, &UBTT_EliteEnemy_Attack::OnIntrruptedCallback);

	// Play Montage
	EliteEnemy->ActivateEliteAttack_Server();

	return EBTNodeResult::InProgress;
}

void UBTT_EliteEnemy_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

void UBTT_EliteEnemy_Attack::CleanUp()
{
	if (EliteEnemy)
	{
		UAnimInstance* AnimInst = EliteEnemy->GetMesh()->GetAnimInstance();
		if (AnimInst)
		{
			AnimInst->OnMontageEnded.Clear();
		}
	}
}

void UBTT_EliteEnemy_Attack::OnIntrruptedCallback(UAnimMontage* Montage, bool Interrupted)
{
	if (Interrupted)
	{
		// Should Check Montage Type ( Don't Stop Task When Interrupted by AttackMontage! )
		UAnimMontage* CurrentMontage = EliteEnemy->GetCurrentMontage();
		if (EliteEnemy && !EliteEnemy->IsEliteAttackMontage(CurrentMontage))
		{
			IsMontageIntrrupted = true;
		}
	}
	else
	{
		IsMontagePlaying = false;
	}
}
