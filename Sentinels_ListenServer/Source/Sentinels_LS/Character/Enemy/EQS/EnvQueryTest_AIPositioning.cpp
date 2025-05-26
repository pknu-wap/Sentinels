// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/EQS/EnvQueryTest_AIPositioning.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "SubSystem/AIPositioningWorldSubsystem.h"

UEnvQueryTest_AIPositioning::UEnvQueryTest_AIPositioning()
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_Point::StaticClass();
	FilterType = EEnvTestFilterType::Match;
}

void UEnvQueryTest_AIPositioning::RunTest(FEnvQueryInstance& QueryInstance) const
{
    UObject* QueryOwner = QueryInstance.Owner.Get();

    UWorld* world = GetWorld();
    if (!world) return;

    UAIPositioningWorldSubsystem* AIPositionSystem = world->GetSubsystem<UAIPositioningWorldSubsystem>();
    if (!AIPositionSystem) return;

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());

        if (AIPositionSystem->IsReservableLocation(ItemLocation))
        {
            It.SetScore(TestPurpose, FilterType, true, true);
        }
        else
        {
            It.SetScore(TestPurpose, FilterType, false, true);
        }

    }
}
