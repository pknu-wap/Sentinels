// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionCondition/Escort/STEscortCondition.h"
#include "System/STGameState.h"
#include "System/Mission/STMissionBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/MissionObject/Interactable/EscortObject/EscortObject.h"
#include "Net/UnrealNetwork.h"

void USTEscortCondition::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USTEscortCondition, Successed);
}

bool USTEscortCondition::IsSatisfied_Implementation()
{
    return Successed;
}

void USTEscortCondition::MissionActivated_Implementation()
{
    Super::MissionActivated_Implementation();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEscortObject::StaticClass(), Actors);
	
	if (Actors.IsEmpty()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("USTEscortCondition : Failed to get Escort objects Array."));
		return;
	}

	// Set Activatable for Random Escort Object
	int rand = UKismetMathLibrary::RandomIntegerInRange(0, Actors.Num() - 1);
	AEscortObject* escortObject = Cast<AEscortObject>(Actors[rand]);
	if (escortObject)
	{
		escortObject->Delegate_MissionConditionUpdate.AddUObject(this, &USTEscortCondition::ConditionUpdated);
	}
}

void USTEscortCondition::MissionDeactivated_Implementation(bool IsCleared)
{
    Super::MissionDeactivated_Implementation(IsCleared);
}

void USTEscortCondition::ConditionUpdated(int ObjectID, bool Success)
{
	// Time Limit Success
	if (Mission)
	{
		Successed = Success;
		Mission->DeactivateMission(Success);
	}

	OnRep_Successed();
	//Delegate_ConditionUpdated.Broadcast();
}

void USTEscortCondition::OnRep_Successed()
{
	Delegate_ConditionUpdated.Broadcast();
}
