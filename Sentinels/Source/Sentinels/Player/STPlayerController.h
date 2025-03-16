// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "STPlayerController.generated.h"

class UInputAction;
class USplineComponent;

class FOnlineSessionSearch;
class FOnlineSessionSearchResult;

USTRUCT()
struct FSessionInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName SessionName;
	FOnlineSessionSearchResult SearchResult;
};

UCLASS()
class SENTINELS_API ASTPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASTPlayerController();

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/*
		Input
	*/
	virtual void SetupInputComponent() override;

	/*
		Move
	*/
	void MoveClick_Started();
	void MoveClick_Triggered();
	void MoveClick_Released();

	void AutoRun();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveClickAction;

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

private:
	FTimerHandle Handle_AutoRun;
	bool bAutoRun = false;
	FVector CachedDestination;
	float FollowTime; // For how long it has been pressed




	/*
		NetWork & Session
	*/
	UFUNCTION(BlueprintCallable, Category = "Session")
	void CreateSession(FName SessionName);

	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION()
	void OnRep_CreateSessionSuccecssful();

	UPROPERTY(ReplicatedUsing = OnRep_CreateSessionSuccecssful)
	bool bCreateSessionSuccecssful;

	UFUNCTION(BlueprintCallable, Category = "Session")
	void FindSessionInfos();

	UFUNCTION()
	void OnFindSessionsComplete(bool bWasSuccessful);

	UFUNCTION()
	void OnRep_SessionInfos();

	UPROPERTY(ReplicatedUsing = OnRep_SessionInfos)
	TArray<FSessionInfo> SessionInfos;

	FOnCreateSessionCompleteDelegate Delegate_CreateSessionComplete;
	FDelegateHandle Handle_CreateSessionComplete;

	FOnFindSessionsCompleteDelegate Delegate_FindSessionComplete;
	FDelegateHandle Handle_FindSessionComplete;


	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;
	FOnlineSessionSearchResult SearchResult;
};
