// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Helix/BTT_Helix_Attack.h"
#include "BTT_Helix_Attack.h"
#include "Character/Enemy/Helix/STHelix.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"


UBTT_Helix_Attack::UBTT_Helix_Attack()
{
	bNotifyTick = true;
	ForceInstancing(true);
	NodeName = FString("Helix Attack");
}

EBTNodeResult::Type UBTT_Helix_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	Helix = Cast<ASTHelix>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Helix) return EBTNodeResult::Failed;

	UAnimInstance* AnimInst = Helix->GetMesh()->GetAnimInstance();
	if (!AnimInst) return EBTNodeResult::Failed;

	// Clean Task Values
	IsMontageIntrrupted = false;
	IsMontagePlaying = true;
	CleanUp();

	// Binding Function
	AnimInst->OnMontageEnded.AddDynamic(this, &UBTT_Helix_Attack::OnIntrruptedCallback);
	
	// Play Montage
	Helix->HelixAttack(AttackType.GetValue());

	return EBTNodeResult::InProgress;
}

void UBTT_Helix_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

void UBTT_Helix_Attack::CleanUp()
{
	if (Helix)
	{
		UAnimInstance* AnimInst = Helix->GetMesh()->GetAnimInstance();
		if (AnimInst)
		{
			AnimInst->OnMontageEnded.Clear();
		}
	}
}

void UBTT_Helix_Attack::OnIntrruptedCallback(UAnimMontage* Montage, bool Interrupted)
{
	IsMontagePlaying = false;
}
