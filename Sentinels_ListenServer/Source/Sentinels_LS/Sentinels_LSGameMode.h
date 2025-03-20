// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Sentinels_LSGameMode.generated.h"

UCLASS(minimalapi)
class ASentinels_LSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASentinels_LSGameMode();

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	void OnRegisterPlayerComplete(FName SessionName, const TArray<FUniqueNetIdRef>& players, bool bWasSuccessful);

public:
	FName CurrentSessionName;

private:
	FOnRegisterPlayersCompleteDelegate Delegate_RegisterPlayerComplete;
	FDelegateHandle Handle_RegisterPlayerComplete;
};



