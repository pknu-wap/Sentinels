// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/STLocalPlayer.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSubsystemTypes.h"
#include "OnlineSubsystemUtils.h"

void USTLocalPlayer::RegisterSelfToSession(FName InSessionName, bool InbWasInvited)
{
	SessionName = InSessionName;
	bWasInvited = InbWasInvited;
}

void USTLocalPlayer::OnPostLogin()
{
	if (bIsRegistered || SessionName == FName())
		return;

	UWorld* world = GetWorld();

	if (world)
	{
		world->GetTimerManager().SetTimer(RegisterHandle, [&, world]()
		{
			if (!world) return; 
			
			const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(world);
			if (sessionInterface)
			{
				if (!sessionInterface->RegisterPlayer(SessionName, *GetPreferredUniqueNetId(), bWasInvited))
				{
					UE_LOG(LogTemp, Display, TEXT("USTLocalPlayer : Failed To Register Player To Session"));
				}
				else
				{
					UE_LOG(LogTemp, Display, TEXT("USTLocalPlayer : Success To Register Player To Session"));
					bIsRegistered = true;
					SessionName = FName();
					world->GetTimerManager().ClearTimer(RegisterHandle);
				}
			}
		}, 0.1, true);
	}
}
