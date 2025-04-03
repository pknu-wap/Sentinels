// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MissionObject/Interactable/InteractableMissionObject.h"
#include "Rift.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API ARift : public AInteractableMissionObject
{
	GENERATED_BODY()

/*
	Interactive Interface
*/
protected:
	virtual void Interact() override;
};
