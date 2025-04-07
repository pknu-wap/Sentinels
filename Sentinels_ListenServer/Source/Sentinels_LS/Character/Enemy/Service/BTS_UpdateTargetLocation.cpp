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

	AActor* Target = Cast<AActor>(TargetObject);
	if (!Target) return;

	/*
		Update Player Location If In Range.
	*/
	FVector ownerLocation = owner->GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();
	float SquredDist = FVector::DistSquared2D(ownerLocation, TargetLocation);

	if (SquredDist > MaxDistance_FromPlayer * MaxDistance_FromPlayer)
	{
		blackBoard->SetValueAsObject(FName("Target"), nullptr);
	}
	else
	{
		blackBoard->SetValueAsVector(FName("TargetLocation"), TargetLocation);
	}

	/*
		Check Enemy Is out from startLocation
	*/
	if (MaxDistance_FromStartLocation > 0)
	{
		FVector startLocation = blackBoard->GetValueAsVector(FName("StartLocation"));
		SquredDist = FVector::DistSquared2D(ownerLocation, startLocation);
		if (SquredDist > MaxDistance_FromStartLocation * MaxDistance_FromStartLocation)
		{
			blackBoard->SetValueAsBool(FName("bShouldReturn"), true);
		}
	}
	
}
