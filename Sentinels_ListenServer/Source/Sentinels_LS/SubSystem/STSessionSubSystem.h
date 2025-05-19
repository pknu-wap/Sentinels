// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "STSessionSubSystem.generated.h"

USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FName SessionName;

	UPROPERTY(BlueprintReadOnly)
	FString OwningUserName;

	UPROPERTY(BlueprintReadOnly)
	int NumPublicConnections;

	UPROPERTY(BlueprintReadOnly)
	int NumOpenPublicConnections;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSTOnRegisterPlayerComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSTOnCreateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSTOnUpdateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSTOnStartSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSTOnEndSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSTOnDestroySessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSTOnFindSessionsComplete, const TArray<FSessionInfo>&, SessionInfos, bool, Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FSTOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);

UCLASS()
class SENTINELS_LS_API USTSessionSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USTSessionSubSystem();
	
	/*
		NetWork & Session
	*/
	UFUNCTION(BlueprintCallable, Category = "Session")
	void RegisterPlayer(FName SessionName);

	UFUNCTION(BlueprintCallable, Category = "Session")
	void CreateSession(FName SessionName, int PublicConnection = 4, bool bUseLan = false);

	UFUNCTION(BlueprintCallable, Category = "Session")
	void StartSession(FName SessionName);

	UFUNCTION(BlueprintCallable, Category = "Session")
	void UpdateSession(FName SessionName);

	UFUNCTION(BlueprintCallable, Category = "Session")
	void FindSessionInfos();

	// ! ! ! Should Register Player Before Join Session ! ! ! 
	UFUNCTION(BlueprintCallable, Category = "Session")
	void JoinGameSession(FName InSessionName);

	UFUNCTION(BlueprintCallable, Category = "Session")
	void TryTravelToCurrentSession(FName SessionName);

	void OnRegisterPlayerComplete(FName SessionName, const TArray<FUniqueNetIdRef>& players, bool bWasSuccessful);
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionCompleted(FName SessionName, bool Successful);
	void OnUpdateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

protected:
	UPROPERTY(BlueprintAssignable)
	FSTOnRegisterPlayerComplete OnRegisterPlayerCompleteEvent;
	UPROPERTY(BlueprintAssignable)
	FSTOnCreateSessionComplete OnCreateSessionCompleteEvent;
	UPROPERTY(BlueprintAssignable)
	FSTOnUpdateSessionComplete OnUpdateSessionCompleteEvent;
	UPROPERTY(BlueprintAssignable)
	FSTOnStartSessionComplete OnStartSessionCompleteEvent;
	UPROPERTY(BlueprintAssignable)
	FSTOnEndSessionComplete OnEndSessionCompleteEvent;
	UPROPERTY(BlueprintAssignable)
	FSTOnDestroySessionComplete OnDestroySessionCompleteEvent;
	UPROPERTY(BlueprintAssignable)
	FSTOnFindSessionsComplete OnFindSessionsCompleteEvent;

	FSTOnJoinSessionComplete OnJoinGameSessionCompleteEvent;

private:
	TArray<FSessionInfo> SessionInfos;

	FOnRegisterPlayersCompleteDelegate Delegate_RegisterPlayerComplete;
	FDelegateHandle Handle_RegisterPlayerComplete;

	FOnCreateSessionCompleteDelegate Delegate_CreateSessionComplete;
	FDelegateHandle Handle_CreateSessionComplete;

	FOnStartSessionCompleteDelegate Delegate_StartSessionComplete;
	FDelegateHandle Handle_StartSessionComplete;

	FOnUpdateSessionCompleteDelegate Delegate_UpdateSessionComplete;
	FDelegateHandle Handle_UpdatetSessionComplete;

	FOnFindSessionsCompleteDelegate Delegate_FindSessionComplete;
	FDelegateHandle Handle_FindSessionComplete;

	FOnJoinSessionCompleteDelegate Delegate_JoinSessionComplete;
	FDelegateHandle Handle_JoinSessionComplete;

	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;
	FOnlineSessionSearchResult SearchResult;
};
