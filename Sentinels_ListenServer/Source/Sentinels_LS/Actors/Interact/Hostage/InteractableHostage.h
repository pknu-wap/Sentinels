// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interact/InteractableCharacter.h"
#include "Actors/MissionObject/MissionObject.h"
#include "InteractableHostage.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API AInteractableHostage : public AInteractableCharacter
{
	GENERATED_BODY()

public:
	void SetHostageID(int InHostageID) { HostageID = InHostageID; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int HostageID = 0;

	/*
		Interactive Interaface
	*/
public:
	virtual void Interact(UInteractComponent* InteractingComponent) override;
	virtual void Interact_Finish(UInteractComponent* InteractingComponent) override;


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
