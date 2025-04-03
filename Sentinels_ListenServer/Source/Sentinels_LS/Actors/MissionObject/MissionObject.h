// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissionObject.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FMissionConditionUpdate, int ObjectID, bool Success);

UCLASS()
class SENTINELS_LS_API AMissionObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissionObject();

public:
	void SetObjectID(int InObjectID) { ObjectID = InObjectID; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	int ObjectID = 0;

public:
	FMissionConditionUpdate Delegate_MissionConditionUpdate;
};
