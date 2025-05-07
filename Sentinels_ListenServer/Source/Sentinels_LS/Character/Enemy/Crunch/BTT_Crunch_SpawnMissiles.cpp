// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Crunch/BTT_Crunch_SpawnMissiles.h"
#include "Character/Enemy/Crunch/STCrunch.h"
#include "AIController.h"

UBTT_Crunch_SpawnMissiles::UBTT_Crunch_SpawnMissiles()
{
	ForceInstancing(true);
	NodeName = FString("Crunch Spawn Missiles");
}

EBTNodeResult::Type UBTT_Crunch_SpawnMissiles::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ASTCrunch* Crunch = Cast<ASTCrunch>(OwnerComp.GetAIOwner()->GetPawn());
	if (Crunch)
	{
		Crunch->StartSpawnMissiles();
	}
	else
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
