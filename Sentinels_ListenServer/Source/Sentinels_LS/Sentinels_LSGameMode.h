// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Sentinels_LSGameMode.generated.h"

UCLASS(minimalapi)
class ASentinels_LSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASentinels_LSGameMode();

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
};



