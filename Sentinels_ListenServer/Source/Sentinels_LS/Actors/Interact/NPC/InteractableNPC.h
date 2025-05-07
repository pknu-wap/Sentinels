// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interact/InteractableCharacter.h"
#include "Actors/MissionObject/MissionObject.h"
#include "InteractableNPC.generated.h"

class UInteractComponent;

UCLASS()
class SENTINELS_LS_API AInteractableNPC : public AInteractableCharacter
{
	GENERATED_BODY()

public:
	void SetNPCID(int InNPCID) { NPCID = InNPCID; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NPCID = 0;
	
/*
	Interactive Interaface
*/
public:
	virtual void Interact_Implementation(UInteractComponent* InteractingComponent) override;
	virtual void Interact_Finish_Implementation(UInteractComponent* InteractingComponent) override;


/*
	Rescue Success
*/
protected:
	void RescueSuccessed();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InteractionHoldTime = 7.5f;

public:
	FMissionConditionUpdate Delegate_MissionConditionUpdate;

private:
	UPROPERTY()
	UInteractComponent* InteractedComponent;

	FTimerHandle Handle_Hold;
};
