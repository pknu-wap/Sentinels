// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/STSessionSubSystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSubsystemTypes.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetStringLibrary.h"
#include "OnlineSubsystemUtils.h"
#include "Kismet/GameplayStatics.h"
#include "System/STGameInstance.h"
#include "Sentinels_LSGameMode.h"
#include "Player/STPlayerController.h"
#include "Player/STLocalPlayer.h"

#pragma region Session

USTSessionSubSystem::USTSessionSubSystem()
	: Delegate_FindSessionComplete(FOnFindSessionsCompleteDelegate::CreateUObject(this, &USTSessionSubSystem::OnFindSessionsComplete)),
	Delegate_CreateSessionComplete(FOnCreateSessionCompleteDelegate::CreateUObject(this, &USTSessionSubSystem::OnCreateSessionComplete)),
	Delegate_JoinSessionComplete(FOnJoinSessionCompleteDelegate::CreateUObject(this, &USTSessionSubSystem::OnJoinSessionCompleted)),
	Delegate_StartSessionComplete(FOnStartSessionCompleteDelegate::CreateUObject(this, &USTSessionSubSystem::OnStartSessionCompleted)),
	Delegate_UpdateSessionComplete(FOnUpdateSessionCompleteDelegate::CreateUObject(this, &USTSessionSubSystem::OnUpdateSessionComplete)),
	Delegate_RegisterPlayerComplete(FOnRegisterPlayersCompleteDelegate::CreateUObject(this, &USTSessionSubSystem::OnRegisterPlayerComplete))
{
}

void USTSessionSubSystem::RegisterPlayer(FName SessionName)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
		return;

	ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!localPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("localPlayer is invalid."));
		return;
	}
	
	Handle_RegisterPlayerComplete = sessionInterface->AddOnRegisterPlayersCompleteDelegate_Handle(Delegate_RegisterPlayerComplete);
	
	if (!sessionInterface->RegisterPlayer(SessionName, *localPlayer->GetPreferredUniqueNetId(), false))
	{
		sessionInterface->ClearOnRegisterPlayersCompleteDelegate_Handle(Handle_RegisterPlayerComplete);
	}
}

void USTSessionSubSystem::OnRegisterPlayerComplete(FName SessionName, const TArray<FUniqueNetIdRef>& players, bool bWasSuccessful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid()) return;

	if (sessionInterface)
	{
		sessionInterface->ClearOnRegisterPlayersCompleteDelegate_Handle(Handle_RegisterPlayerComplete);
	}

	OnRegisterPlayerCompleteEvent.Broadcast(bWasSuccessful);
}

void USTSessionSubSystem::CreateSession(FName SessionName, int PublicConnection, bool bUseLan)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
		return;

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->NumPrivateConnections = 0;
	LastSessionSettings->NumPublicConnections = PublicConnection;
	LastSessionSettings->bAllowInvites = true;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
	LastSessionSettings->bIsDedicated = false;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bIsLANMatch = bUseLan;
	LastSessionSettings->bShouldAdvertise = true;

	LastSessionSettings->Set(FName("SessionName"), SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);

	Handle_CreateSessionComplete = sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(Delegate_CreateSessionComplete);

	ULocalPlayer* localPlayer =  GetWorld()->GetFirstLocalPlayerFromController();
	if (!localPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("HostingPlayer is invalid."));
		return;
	}

	if (!sessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), SessionName, *LastSessionSettings))
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(Handle_CreateSessionComplete);

		// OnCreateSessionCompleteEvent.Broadcast(false);
	}
}

void USTSessionSubSystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid()) return;

	if (sessionInterface)
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(Handle_CreateSessionComplete);
	}

	OnCreateSessionCompleteEvent.Broadcast(bWasSuccessful);

	USTGameInstance* GameInst = Cast<USTGameInstance>(GetGameInstance());
	if (GameInst)
	{
		GameInst->CurrentSessionName = SessionName;
	}

	ASentinels_LSGameMode* GameMode = Cast<ASentinels_LSGameMode>(UGameplayStatics::GetGameMode(GameInst));
	if (GameMode)
	{
		GameMode->CurrentSessionName = SessionName;
	}

	if (bWasSuccessful)
	{
		RegisterPlayer(SessionName);
		StartSession(SessionName);
	}
}

void USTSessionSubSystem::StartSession(FName SessionName)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		OnStartSessionCompleteEvent.Broadcast(false);
		return;
	}

	Handle_StartSessionComplete =
		sessionInterface->AddOnStartSessionCompleteDelegate_Handle(Delegate_StartSessionComplete);

	if (!sessionInterface->StartSession(SessionName))
	{
		sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(Handle_StartSessionComplete);

		OnStartSessionCompleteEvent.Broadcast(false);
	}
}

void USTSessionSubSystem::OnStartSessionCompleted(FName SessionName, bool bWasSuccessful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(Handle_StartSessionComplete);
	}

	OnStartSessionCompleteEvent.Broadcast(bWasSuccessful);

	// If the start was successful, we can open a NewMap if we want. Make sure to use "listen" as a parameter!
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "PreviewSceneIndoor", true, "listen");
	}
}

void USTSessionSubSystem::UpdateSession(FName SessionName)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		OnUpdateSessionCompleteEvent.Broadcast(false);
		return;
	}

	TSharedPtr<FOnlineSessionSettings> updatedSessionSettings;

	updatedSessionSettings->Set(SETTING_MAPNAME, FString("Updated Level Name"), EOnlineDataAdvertisementType::ViaOnlineService);

	Handle_UpdatetSessionComplete =
		sessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(Delegate_UpdateSessionComplete);

	if (!sessionInterface->UpdateSession(SessionName, *updatedSessionSettings))
	{
		sessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(Handle_UpdatetSessionComplete);
		OnUpdateSessionCompleteEvent.Broadcast(false);
	}
}

void USTSessionSubSystem::OnUpdateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(Handle_UpdatetSessionComplete);
	}

	OnUpdateSessionCompleteEvent.Broadcast(bWasSuccessful);

	if (bWasSuccessful)
	{

	}
}

void USTSessionSubSystem::FindSessionInfos()
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		OnFindSessionsCompleteEvent.Broadcast(TArray<FSessionInfo>(), false);
		return;
	}

	Handle_FindSessionComplete =
		sessionInterface->AddOnFindSessionsCompleteDelegate_Handle(Delegate_FindSessionComplete);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = 100;
	LastSessionSearch->bIsLanQuery = true;
	LastSessionSearch->TimeoutInSeconds = 1.f;

	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	ULocalPlayer* localPlayer =  GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->FindSessions(*localPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(Handle_FindSessionComplete);
		OnFindSessionsCompleteEvent.Broadcast(TArray<FSessionInfo>(), false);
	}

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [&]() {this->OnFindSessionsComplete(true); }, 1.f, false);
}

void USTSessionSubSystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (!bWasSuccessful) return;

	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		return;
	}

	if (sessionInterface)
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(Handle_FindSessionComplete);
	}

	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		OnFindSessionsCompleteEvent.Broadcast(TArray<FSessionInfo>(), bWasSuccessful);
		return;
	}

	SessionInfos.Empty();
	for (auto& result : LastSessionSearch->SearchResults)
	{
		FSessionInfo info;

		// Session Name
		FString sessionString;
		result.Session.SessionSettings.Get(FName("SessionName"), sessionString);
		
		info.SessionName = UKismetStringLibrary::Conv_StringToName(sessionString);
		info.NumOpenPublicConnections = result.Session.NumOpenPublicConnections;
		info.NumPublicConnections = result.Session.SessionSettings.NumPublicConnections;
		info.OwningUserName = result.Session.OwningUserName;
		// result.Session.SessionInfo->

		SessionInfos.Push(info);
	}

	for (auto& info : SessionInfos)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Session Founded!!")));
		UE_LOG(LogTemp, Display, TEXT("Session Detected : %s"), *info.SessionName.ToString());
	}

	OnFindSessionsCompleteEvent.Broadcast(SessionInfos, bWasSuccessful);
}

void USTSessionSubSystem::JoinGameSession(FName InSessionName)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		return;
	}

	Handle_JoinSessionComplete =
		sessionInterface->AddOnJoinSessionCompleteDelegate_Handle(Delegate_JoinSessionComplete);

	ULocalPlayer* localPlayer =  GetWorld()->GetFirstLocalPlayerFromController();

	if (!LastSessionSearch.IsValid())
	{
		return;
	}

	for (auto& result : LastSessionSearch->SearchResults)
	{
		// Session Name
		FString sessionString; FName SessionName;
		result.Session.SessionSettings.Get(FName("SessionName"), sessionString);
		SessionName = UKismetStringLibrary::Conv_StringToName(sessionString);

		if (SessionName.IsEqual(InSessionName))
		{
			if (!sessionInterface->JoinSession(*localPlayer->GetPreferredUniqueNetId(), SessionName, result))
			{
				sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(Handle_JoinSessionComplete);
			}
			break;
		}
	}
}

void USTSessionSubSystem::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(Handle_JoinSessionComplete);
	}

	switch (Result)
	{
	case EOnJoinSessionCompleteResult::Success:
		UE_LOG(LogTemp, Display, TEXT("OnJoinSessionCompleted : Success"));
		break;
	case EOnJoinSessionCompleteResult::SessionIsFull:
		UE_LOG(LogTemp, Display, TEXT("OnJoinSessionCompleted : SessionIsFull"));
		break;
	case EOnJoinSessionCompleteResult::SessionDoesNotExist:
		UE_LOG(LogTemp, Display, TEXT("OnJoinSessionCompleted : SessionDoesNotExist"));
		break;
	case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
		UE_LOG(LogTemp, Display, TEXT("OnJoinSessionCompleted : CouldNotRetrieveAddress"));
		break;
	case EOnJoinSessionCompleteResult::AlreadyInSession:
		UE_LOG(LogTemp, Display, TEXT("OnJoinSessionCompleted : AlreadyInSession"));
		break;
	case EOnJoinSessionCompleteResult::UnknownError:
		UE_LOG(LogTemp, Display, TEXT("OnJoinSessionCompleted : UnknownError"));
		break;
	default:
		break;
	}

	USTGameInstance* GameInst = Cast<USTGameInstance>(GetGameInstance());
	if (GameInst)
	{
		GameInst->CurrentSessionName = SessionName;
	}

	ASentinels_LSGameMode* GameMode = Cast<ASentinels_LSGameMode>(UGameplayStatics::GetGameMode(GameInst));
	if (GameMode)
	{
		GameMode->CurrentSessionName = SessionName;
	}

	USTLocalPlayer* LocalPlayer = Cast<USTLocalPlayer>(UGameplayStatics::GetPlayerController(this, 0)->GetLocalPlayer());
	if (LocalPlayer)
	{
		LocalPlayer->RegisterSelfToSession(SessionName, false);
	}

	/*ASTPlayerController* PC = Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC)
	{
		PC->RegisterSelfToSession(SessionName);
	}*/

	/*ASTPlayerController* PC = Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC)
	{
		PC->RegisterSelfToSession(SessionName);
	}*/

	TryTravelToCurrentSession(SessionName);
}

void USTSessionSubSystem::TryTravelToCurrentSession(FName SessionName)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		return;
	}

	FString connectString;
	if (!sessionInterface->GetResolvedConnectString(SessionName, connectString))
	{
		return;
	}

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	playerController->ClientTravel(connectString, TRAVEL_Absolute);
	return;
}

#pragma endregion