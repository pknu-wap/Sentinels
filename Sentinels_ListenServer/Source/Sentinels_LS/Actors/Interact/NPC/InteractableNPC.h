// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interact/InteractableCharacter.h"
#include "Actors/MissionObject/MissionObject.h"
#include "InteractableNPC.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API AInteractableNPC : public AInteractableCharacter
{
	GENERATED_BODY()
	
public:
	virtual void Interact() override;

public:
	void SetNPCID(int InNPCID) { NPCID = InNPCID; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NPCID = 0;

public:
	FMissionConditionUpdate Delegate_MissionConditionUpdate;
};
