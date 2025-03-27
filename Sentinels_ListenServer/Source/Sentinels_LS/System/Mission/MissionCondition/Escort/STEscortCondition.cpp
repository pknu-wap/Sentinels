// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionCondition/Escort/STEscortCondition.h"

void USTEscortCondition::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

bool USTEscortCondition::IsSatisfied()
{
    return Successed;
}

void USTEscortCondition::MissionActivated()
{
    Super::MissionActivated();
}

void USTEscortCondition::MissionDeactivated(bool IsCleared)
{
    Super::MissionDeactivated(IsCleared);
}

void USTEscortCondition::UpdateEscortInfo(int ObjectID, bool IsSuccessed)
{
    Successed = IsSuccessed;
}
