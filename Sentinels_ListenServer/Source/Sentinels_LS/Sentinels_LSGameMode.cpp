// Copyright Epic Games, Inc. All Rights Reserved.

#include "Sentinels_LSGameMode.h"
#include "Sentinels_LSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSubsystemTypes.h"
#include "GameFramework/PlayerState.h"
#include "System/STGameInstance.h"

ASentinels_LSGameMode::ASentinels_LSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ASentinels_LSGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (!OnlineSub) return;

    IOnlineSessionPtr SessionInterface = OnlineSub->GetSessionInterface();
    if (!SessionInterface.IsValid()) return;
}
