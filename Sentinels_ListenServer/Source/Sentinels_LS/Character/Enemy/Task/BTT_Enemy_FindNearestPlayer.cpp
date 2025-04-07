// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Task/BTT_Enemy_FindNearestPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/STPlayerCharacter.h"

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

	AActor* CurrentTarget = Cast<AActor>(blackBoard->GetValueAsObject(FName("Target")));

	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(Enemy, ASTPlayerCharacter::StaticClass(), Players);
	if(Players.IsEmpty()) return EBTNodeResult::Failed;

	float MinDist = 100000;
	AActor* NearestPlayer = nullptr;
	for (auto player : Players)
	{
		if (player == CurrentTarget) continue;

	 	float Dist = FVector::Dist(Enemy->GetActorLocation(), player->GetActorLocation());
		if (Dist < MinDist)
		{
			MinDist = Dist;
			NearestPlayer = player;
		}
	}

	if (blackBoard && NearestPlayer)
	{
		blackBoard->SetValueAsObject(FName("Target"), NearestPlayer);
		blackBoard->SetValueAsVector(FName("TargetLocation"), NearestPlayer->GetActorLocation());
		blackBoard->SetValueAsBool(FName("IsStucked"), false);
	}

	return EBTNodeResult::Succeeded;
}
