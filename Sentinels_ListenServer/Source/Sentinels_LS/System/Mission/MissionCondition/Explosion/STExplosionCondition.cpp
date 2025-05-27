// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionCondition/Explosion/STExplosionCondition.h"
#include "SubSystem/STEventSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "System/STGameState.h"
#include "System/Mission/STMissionBase.h"
#include "Kismet/GameplayStatics.h"
#include "STGameplayTags.h"

bool USTExplosionCondition::IsSatisfied_Implementation()
{
	return bIsExplosionOccured;
}

void USTExplosionCondition::MissionActivated_Implementation()
{
	bIsExplosionOccured = false;

	UWorld* world = GetWorld();
	if (world)
	{
		USTEventSubsystem* EventSubSystem = world->GetSubsystem<USTEventSubsystem>();
		EventSubSystem->Delegate_EventOccur.AddDynamic(this, &USTExplosionCondition::OnEventOccur);
	}
}

void USTExplosionCondition::OnEventOccur(FGameplayTag InTag)
{
	if (InTag != FSTGameplayTags::Get().SubMission_Explosion)
	{
		return;
	}

	bIsExplosionOccured = true;

	if (Mission)
	{
		Mission->DeactivateMission(true);
	}

	Delegate_ConditionUpdated.Broadcast();
}
