// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MissionObject/Interactable/InteractableMissionObject.h"
#include "Rift.generated.h"

class UInteractComponent;

UCLASS()
class SENTINELS_LS_API ARift : public AInteractableMissionObject
{
	GENERATED_BODY()

/*
	Interactive Interface
*/
protected:
	virtual void Interact(UInteractComponent* InteractingComponent) override;
	virtual void Interact_Finish(UInteractComponent* InteractingComponent) override;

/*
	Repair Success
*/
protected:
	void RepairSuccessed();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InteractionHoldTime = 7.5f;

private:
	UPROPERTY()
	UInteractComponent* InteractedComponent;

	FTimerHandle Handle_Hold;
};
