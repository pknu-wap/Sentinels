// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interact/InteractableActor.h"
#include "InteractableRift.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API AInteractableRift : public AInteractableActor
{
	GENERATED_BODY()
	
public:
	virtual void Interact() override;
	virtual void ShowInteractiveUI() override;
};
