// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionCondition/Explosion/STExplosionCondition.h"
#include "SubSystem/STEventSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "System/STGameState.h"
#include "System/Mission/STMissionBase.h"
#include "Kismet/GameplayStatics.h"

bool USTExplosionCondition::IsSatisfied()
{
	return bIsExplosionOccured;
}

void USTExplosionCondition::MissionActivated()
{
	bIsExplosionOccured = false;

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (GameInstance)
	{
		USTEventSubsystem* EventSubSystem = GameInstance->GetSubsystem<USTEventSubsystem>();
		EventSubSystem->Delegate_ExplosionEvent.AddDynamic(this, &USTExplosionCondition::OnEventOccur);
	}
}

void USTExplosionCondition::OnEventOccur(FGameplayTag InTag)
{
	bIsExplosionOccured = true;

	ASTGameState* GameState = Cast<ASTGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		USTMissionBase* Mission = GameState->GetMission(MissionTag);
		if (Mission)
		{
			Mission->DeactivateMission(true);
		}
	}

	Delegate_ConditionUpdated.Broadcast();
}
