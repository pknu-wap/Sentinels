// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractiveInterface.generated.h"

class UInteractComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractiveInterface : public UInterface
{
	GENERATED_BODY()
};

class SENTINELS_LS_API IInteractiveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Interact(UInteractComponent* InteractingComponent) = 0;
	virtual void Interact_Finish(UInteractComponent* InteractingComponent) = 0;
	virtual void ShowInteractiveUI(UInteractComponent* InteractingComponent) = 0;
	virtual void HideInteractiveUI(UInteractComponent* InteractingComponent) = 0;
};
