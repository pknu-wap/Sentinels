// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/EQS/EnvQueryTest_CapsuleTrace.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "SubSystem/AIPositioningWorldSubsystem.h"

UEnvQueryTest_CapsuleTrace::UEnvQueryTest_CapsuleTrace()
{
	Cost = EEnvTestCost::High;
	ValidItemType = UEnvQueryItemType_Point::StaticClass();
    CapsuleRadius.DefaultValue = 42.f;
    FilterType = EEnvTestFilterType::Match;
}

void UEnvQueryTest_CapsuleTrace::RunTest(FEnvQueryInstance& QueryInstance) const
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
            // Sphere overlap
            TArray<FOverlapResult> Overlaps;
            FCollisionQueryParams Params;
            Params.AddIgnoredActor(Cast<AActor>(QueryOwner));

            QueryInstance.World->OverlapMultiByObjectType(
                Overlaps,
                ItemLocation,
                FQuat::Identity,
                FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),
                FCollisionShape::MakeSphere(CapsuleRadius.GetValue()),
                Params
            );

            bool bOccupied = false;
            for (const FOverlapResult& Result : Overlaps)
            {
                AActor* actor = Result.GetActor();
                if (actor)
                {
                    UE_LOG(LogTemp, Display, TEXT("%s"), *actor->GetName());
                    bOccupied = true;
                    break;
                }
                else
                {
                    UE_LOG(LogTemp, Display, TEXT("Failed to get actor name"));
                }
            }

            It.SetScore(TestPurpose, FilterType, !bOccupied, true);
        }
        else
        {
            It.SetScore(TestPurpose, FilterType, false, true);
        }
        
    }
}
