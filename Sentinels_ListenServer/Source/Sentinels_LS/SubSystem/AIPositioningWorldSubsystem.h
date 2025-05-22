// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AIPositioningWorldSubsystem.generated.h"

USTRUCT()
struct FAIGridPoint
{
	GENERATED_BODY()

	FAIGridPoint() : x(0), y(0) {};
	FAIGridPoint(int inX, int inY) : x(inX), y(inY) {};

	bool operator==(const FAIGridPoint& Other) const
	{
		return Other.x == x && Other.y == y;
	}

	FORCEINLINE friend uint32 GetTypeHash(const FAIGridPoint& Point)
	{
		return HashCombine(::GetTypeHash(Point.x), ::GetTypeHash(Point.y));
	}

	int x, y;
};

//USTRUCT()
//struct FAIGridCell
//{
//	GENERATED_BODY()
//
//	FAIGridCell(const FAIGridPoint& InLocation, bool InOccupied, UObject* InObject)
//		: WorldPosition(InLocation), bIsOccupied(InOccupied), OccupiedBy(InObject) {}
//
//	bool operator==(const FAIGridCell& Other) const
//	{
//		return Other.WorldPosition == WorldPosition;
//	}
//
//	FAIGridPoint WorldPosition;
//	bool bIsOccupied = false;
//	UObject* OccupiedBy = nullptr;
//};

UCLASS()
class SENTINELS_LS_API UAIPositioningWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	UFUNCTION(BlueprintCallable)
	bool IsReservableLocation(const FVector& InLocation) const;

	UFUNCTION(BlueprintCallable)
	void ReserveLocation(const FVector& InLocation, UObject* Requester);

	UFUNCTION(BlueprintCallable)
	void ReleaseLocation(const FVector& InLocation, UObject* Requester);

	FAIGridPoint GetGridPoint(const FVector& InLocation) const;

protected:
	UPROPERTY()
	TSet<FAIGridPoint> WorldGridMap;

	UPROPERTY()
	int CellSize = 100;
};
