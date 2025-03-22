// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Interface.h"
#include "STMissionBase.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USTMissionBase : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class SENTINELS_LS_API ISTMissionBase
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	virtual void ActivateMission() = 0;
	virtual void DeactivateMission() = 0;
	virtual bool IsMissionCleared() = 0;
};