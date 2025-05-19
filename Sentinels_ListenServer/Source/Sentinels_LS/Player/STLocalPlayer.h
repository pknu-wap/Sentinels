// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "STLocalPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API USTLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()
	

/*
	Register Self To Session
*/
public:
	void RegisterSelfToSession(FName InSessionName, bool InbWasInvited);
	void OnPostLogin();

public:
	FName GetSessionName() const { return SessionName; }
	bool IsRegistered() const { return bIsRegistered; }

public:
	UPROPERTY()
	bool bIsRegistered;

protected:
	UPROPERTY()
	FName SessionName;

	UPROPERTY()
	bool bWasInvited;

private:
	FTimerHandle RegisterHandle;
};
