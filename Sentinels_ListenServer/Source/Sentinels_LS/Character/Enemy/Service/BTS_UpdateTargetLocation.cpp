// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Service/BTS_UpdateTargetLocation.h"
#include "Character/Enemy/STEnemyBase_AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


UBTS_UpdateTargetLocation::UBTS_UpdateTargetLocation()
{
	NodeName = "Update Target Location";
	ForceInstancing(true);
}

void UBTS_UpdateTargetLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* blackBoard = OwnerComp.GetBlackboardComponent();
	AActor* owner = OwnerComp.GetOwner();
	if (!blackBoard || !owner) return;

	UObject* TargetObject = blackBoard->GetValueAsObject(FName("Target"));
	if (!TargetObject) return;

	APawn* Target = Cast<APawn>(TargetObject);
	if (!Target) return;

	FVector ownerLocation = owner->GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();
	float Dist = FVector::Dist2D(ownerLocation, TargetLocation);

	// Update Player Location If In Range.
	if (Dist > MaxDistance)
	{
		blackBoard->SetValueAsObject(FName("Target"), nullptr);
	}
	else
	{
		blackBoard->SetValueAsVector(FName("TargetLocation"), TargetLocation);
	}
}
