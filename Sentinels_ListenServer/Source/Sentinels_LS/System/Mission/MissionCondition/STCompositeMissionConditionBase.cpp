// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionCondition/STCompositeMissionConditionBase.h"
#include "Net/UnrealNetwork.h"
#include "STMissionConditionBase.h"

void USTCompositeMissionConditionBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USTCompositeMissionConditionBase, SubConditions);
}

void USTCompositeMissionConditionBase::PostInitProperties()
{
	Super::PostInitProperties();

	AActor* owner = GetTypedOuter<AActor>();
	if (owner)
	{
		owner->AddReplicatedSubObject(this);

		for (auto& ConditionClass : SubConditionClasses)
		{
			USTMissionConditionBase* NewCondition = NewObject<USTMissionConditionBase>(owner, ConditionClass);
			if (NewCondition)
			{
				owner->AddReplicatedSubObject(NewCondition);
				SubConditions.Push(NewCondition);
			}
		}
	}
}

bool USTCompositeMissionConditionBase::IsSatisfied()
{
	if (CombineType == EConditionCombineType::AND)
	{
		for (auto& Cond : SubConditions)
		{
			if (!Cond->IsSatisfied())
				return false;
		}
		return true;
	}
	else // OR
	{
		for (auto& Cond : SubConditions)
		{
			if (Cond->IsSatisfied())
				return true;
		}
		return false;
	}

	return false;
}
