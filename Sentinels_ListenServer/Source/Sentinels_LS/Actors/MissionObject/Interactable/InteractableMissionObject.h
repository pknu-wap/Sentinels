// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MissionObject/MissionObject.h"
#include "Interfaces/InteractiveInterface.h"
#include "InteractableMissionObject.generated.h"

class UInteractComponent;

UCLASS()
class SENTINELS_LS_API AInteractableMissionObject : public AMissionObject, public IInteractiveInterface
{
	GENERATED_BODY()

public:
	AInteractableMissionObject();

/*
	Actor Interface	
*/
protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

/*
	Interactive Interface
*/
protected:
	virtual void Interact_Implementation(UInteractComponent* InteractingComponent) override;
	virtual void Interact_Finish_Implementation(UInteractComponent* InteractingComponent) override;
	virtual void ShowInteractiveUI_Implementation(UInteractComponent* InteractingComponent) override;
	virtual void HideInteractiveUI_Implementation(UInteractComponent* InteractingComponent) override;

	virtual bool IsInteractable_Implementation() override { return bIsInteractable; };

protected:
	UPROPERTY(EditAnywhere, Category = "Debug")
	TSubclassOf<UUserWidget> InteractWidgetClass_ForDebug;

	UPROPERTY()
	UUserWidget* InteractWidget_ForDebug;

protected:
	UPROPERTY(Replicated,  VisibleAnywhere)
	bool bIsInteractable = true;
};
