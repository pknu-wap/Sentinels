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
#include "Player/STPlayerController.h"
#include "Player/STPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "System/STGameState.h"
#include "SubSystem/STGameTravelDataSubsystem.h"
#include "SubSystem/STWorldSpawnSubsystem.h"
#include "Player/STLocalPlayer.h"
#include "Kismet/GameplayStatics.h"

ASentinels_LSGameMode::ASentinels_LSGameMode()
	: Delegate_RegisterPlayerComplete(FOnRegisterPlayersCompleteDelegate::CreateUObject(this, &ASentinels_LSGameMode::OnRegisterPlayerComplete))
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}

	GameStateClass = ASTGameState::StaticClass();
}

void ASentinels_LSGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	
}

void ASentinels_LSGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UWorld* world = GetWorld();
	if (world)
	{
		USTWorldSpawnSubsystem* WorldSpawnSystem = world->GetSubsystem<USTWorldSpawnSubsystem>();
		if (WorldSpawnSystem)
		{
			WorldSpawnSystem->PlayerNumUpdated(world->GetNumPlayerControllers());
		}
	}


    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (!OnlineSub) return;

    IOnlineSessionPtr SessionInterface = OnlineSub->GetSessionInterface();
    if (!SessionInterface.IsValid()) return;

	APlayerState* PlayerState = NewPlayer->PlayerState;
	if (!PlayerState) return;

	if (ASTPlayerController* PC = Cast<ASTPlayerController>(NewPlayer))
	{
		PC->RegisterSelfToSession_Client();
	}

	//USTLocalPlayer* LocalPlayer = Cast<USTLocalPlayer>(NewPlayer->GetLocalPlayer());
	//if (LocalPlayer)
	//{
	//	LocalPlayer->OnPostLogin();
	//	// SessionInterface->RegisterPlayer(LocalPlayer->SessionName, *LocalPlayer->GetPreferredUniqueNetId(), false);
	//}

	if (PlayerState)
	{
		SessionInterface->AddOnRegisterPlayersCompleteDelegate_Handle(Delegate_RegisterPlayerComplete);

		if (!SessionInterface->RegisterPlayer(CurrentSessionName, *PlayerState->GetUniqueId(), false))
		{
			SessionInterface->ClearOnRegisterPlayersCompleteDelegate_Handle(Handle_RegisterPlayerComplete);
		}
	}

}

void ASentinels_LSGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	ASTPlayerController* playerController = Cast<ASTPlayerController>(NewPlayer);

	USTGameTravelDataSubsystem* gameTravelData = GetGameInstance()->GetSubsystem<USTGameTravelDataSubsystem>();
	FPlayerInfo playerInfo = gameTravelData->LoadPlayerInfo(NewPlayer->PlayerState->GetUniqueId());

	playerController->UpdatePlayerClass(playerInfo.PlayerClass);
}

void ASentinels_LSGameMode::OnRegisterPlayerComplete(FName SessionName, const TArray<FUniqueNetIdRef>& players, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub) return;

	IOnlineSessionPtr SessionInterface = OnlineSub->GetSessionInterface();
	if (!SessionInterface.IsValid()) return;

	SessionInterface->ClearOnRegisterPlayersCompleteDelegate_Handle(Handle_RegisterPlayerComplete);
}

UClass* ASentinels_LSGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	ASTPlayerController* STController = Cast<ASTPlayerController>(InController);
	if (STController && STController->GetDefaultPlayerClass())
	{
		return STController->GetDefaultPlayerClass();
	}

	return DefaultPawnClass;
}
