// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Task/BTT_Enemy_FindNearestPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/STPlayerCharacter.h"
#include "Character/Enemy/STEnemyBase_AIController.h"

UBTT_Enemy_FindNearestPlayer::UBTT_Enemy_FindNearestPlayer()
{
	bNotifyTick = false;
	ForceInstancing(true);
	NodeName = FString("Enemy Find Nearest Reachable Player");
}

EBTNodeResult::Type UBTT_Enemy_FindNearestPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* blackBoard = OwnerComp.GetBlackboardComponent();
	APawn* Enemy = OwnerComp.GetAIOwner()->GetPawn();
	if (!Enemy) return EBTNodeResult::Failed;

	ASTEnemyBase_AIController* controller = Cast<ASTEnemyBase_AIController>(Enemy->GetController());
	if (controller)
	{
		AActor* storedTarget = controller->GetStoredTarget();
		if (!storedTarget)
		{
			return  EBTNodeResult::Succeeded;
		}
	}

	AActor* CurrentTarget = Cast<AActor>(blackBoard->GetValueAsObject(FName("Target")));

	int numOfPlayers = UGameplayStatics::GetNumPlayerControllers(Enemy);

	float MinDist = 100000;
	ASTPlayerCharacter* NearestPlayer = nullptr;
	for (int i = 0; i < numOfPlayers; i++)
	{
		ASTPlayerCharacter* player = Cast<ASTPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, i));

		if (player == CurrentTarget) continue;

		if (player && !player->bIsDied)
		{
			float Dist = FVector::Dist(Enemy->GetActorLocation(), player->GetActorLocation());
			if (NearestPlayer == nullptr || Dist < MinDist)
			{
				MinDist = Dist;
				NearestPlayer = player;
			}
		}
	}

	if (controller && NearestPlayer)
	{
		controller->SetTarget(NearestPlayer);
	}

	return EBTNodeResult::Succeeded;
}
