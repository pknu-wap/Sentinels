// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MissionObject/MissionObject.h"
#include "Interfaces/InteractiveInterface.h"
#include "InteractableMissionObject.generated.h"

UCLASS()
class SENTINELS_LS_API AInteractableMissionObject : public AMissionObject, public IInteractiveInterface
{
	GENERATED_BODY()

/*
	Interactive Interface
*/
protected:
	virtual void Interact() override;
	virtual void ShowInteractiveUI() override;
	virtual void HideInteractiveUI() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Debug")
	TSubclassOf<UUserWidget> InteractWidgetClass_ForDebug;

	UPROPERTY()
	UUserWidget* InteractWidget_ForDebug;
};
