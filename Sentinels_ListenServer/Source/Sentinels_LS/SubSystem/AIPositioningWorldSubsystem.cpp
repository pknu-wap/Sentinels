// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/AIPositioningWorldSubsystem.h"

void UAIPositioningWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	CellSize = 100;
}

bool UAIPositioningWorldSubsystem::IsReservableLocation(const FVector& InLocation) const
{
	FAIGridPoint gridPoint = GetGridPoint(InLocation);
	return !WorldGridMap.Contains(gridPoint);
}

void UAIPositioningWorldSubsystem::ReserveLocation(const FVector& InLocation, UObject* Requester)
{
	WorldGridMap.Add(GetGridPoint(InLocation));
}

void UAIPositioningWorldSubsystem::ReleaseLocation(const FVector& InLocation, UObject* Requester)
{
	FAIGridPoint gridPoint = GetGridPoint(InLocation);
	WorldGridMap.Remove(gridPoint);	
}

FAIGridPoint UAIPositioningWorldSubsystem::GetGridPoint(const FVector& InLocation) const
{
	return FAIGridPoint(InLocation.X / CellSize, InLocation.Y / CellSize);
}
