// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/STPlayerController.h"
#include "EnhancedInputComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Components/SplineComponent.h"
#include "DrawDebugHelpers.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSubsystemTypes.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetStringLibrary.h"
#include "OnlineSubsystemUtils.h"

ASTPlayerController::ASTPlayerController()
	: Delegate_FindSessionComplete(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ASTPlayerController::OnFindSessionsComplete)),
	Delegate_CreateSessionComplete(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ASTPlayerController::OnCreateSessionComplete))

{
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void ASTPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ASTPlayerController, bCreateSessionSuccecssful);
	DOREPLIFETIME(ASTPlayerController, SessionInfos);
}

void ASTPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveClickAction, ETriggerEvent::Started, this, &ASTPlayerController::MoveClick_Started);
		EnhancedInputComponent->BindAction(MoveClickAction, ETriggerEvent::Triggered, this, &ASTPlayerController::MoveClick_Triggered);
		EnhancedInputComponent->BindAction(MoveClickAction, ETriggerEvent::Completed, this, &ASTPlayerController::MoveClick_Released);
		EnhancedInputComponent->BindAction(MoveClickAction, ETriggerEvent::Canceled, this, &ASTPlayerController::MoveClick_Released);
	}
}

void ASTPlayerController::MoveClick_Started()
{
	StopMovement();
	GetWorldTimerManager().ClearTimer(Handle_AutoRun);
}

void ASTPlayerController::MoveClick_Triggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void ASTPlayerController::MoveClick_Released()
{
	// If it was a short press
	APawn* ControlledPawn = GetPawn();
	if (FollowTime <= ShortPressThreshold)
	{
		if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetTargetLocation(), CachedDestination))
		{
			Spline->ClearSplinePoints();

			for (const FVector& point : NavPath->PathPoints)
			{
				Spline->AddSplinePoint(point, ESplineCoordinateSpace::World);
				DrawDebugSphere(GetWorld(), point, 10.f, 8, FColor::Green, false, 5.f);
			}

			GetWorldTimerManager().SetTimer(Handle_AutoRun, this, &ASTPlayerController::AutoRun, 0.015f, true);
		}
	}

	FollowTime = 0.f;
}

void ASTPlayerController::AutoRun()
{
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector NextLocation = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector NextDirection = Spline->FindDirectionClosestToWorldLocation(NextLocation, ESplineCoordinateSpace::World);

		ControlledPawn->AddMovementInput(NextDirection);

		const float DistanceDestination = (NextLocation - CachedDestination).Length();
		if (DistanceDestination <= AutoRunAcceptanceRadius)
		{
			StopMovement();
			GetWorldTimerManager().ClearTimer(Handle_AutoRun);
		}
	}
}

#pragma region Session

void ASTPlayerController::CreateSession_Server_Implementation(FName SessionName)
{
	if (HasAuthority())
	{
		IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
		if (OnlineSub)
		{
			IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
			if (Sessions.IsValid())
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("IOnlineSubsystem : %s"), *OnlineSub->GetSubsystemName().ToString()));

				TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());

				/* Editor Version */ 
				/*SessionSettings->bIsDedicated = false;
				SessionSettings->bIsLANMatch = OnlineSub->GetSubsystemName() == "NULL" ? true : false;
				SessionSettings->NumPublicConnections = 4;
				SessionSettings->bShouldAdvertise = true;
				SessionSettings->bUseLobbiesIfAvailable = true;
				SessionSettings->bAllowJoinViaPresence = false;
				SessionSettings->bAllowJoinInProgress = false;
				SessionSettings->bUsesPresence = false;*/

				/* Package Version */

				SessionSettings->NumPublicConnections = 4;
				SessionSettings->bIsDedicated = false;
				SessionSettings->bAllowInvites = true;
				SessionSettings->bAllowJoinViaPresence = true;
				SessionSettings->bAllowJoinInProgress = true;
				SessionSettings->bIsLANMatch = OnlineSub->GetSubsystemName() == "NULL" ? true : false;
				SessionSettings->bShouldAdvertise = true;
				SessionSettings->bUsesPresence = true;
				SessionSettings->bUseLobbiesIfAvailable = true;
				

				// SessionSettings->Set(FName("SessionName"), SessionName.ToString());

				Sessions->AddOnCreateSessionCompleteDelegate_Handle(Delegate_CreateSessionComplete);

				ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
				Sessions->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), SessionName, *SessionSettings);
			}
		}
	}
}

void ASTPlayerController::CreateSession_Local(FName SessionName)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
		return;
	
	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->NumPrivateConnections = 0;
	LastSessionSettings->NumPublicConnections = 5;
	LastSessionSettings->bAllowInvites = true;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
	LastSessionSettings->bIsDedicated = false;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bIsLANMatch = true;
	LastSessionSettings->bShouldAdvertise = true;

	LastSessionSettings->Set(FName("SessionName"), SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);

	Handle_CreateSessionComplete = sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(Delegate_CreateSessionComplete);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(Handle_CreateSessionComplete);

		// OnCreateSessionCompleteEvent.Broadcast(false);
	}
}

void ASTPlayerController::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid()) return;

	if (sessionInterface)
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(Handle_CreateSessionComplete);
	}
}

void ASTPlayerController::OnRep_CreateSessionSuccecssful()
{
	if (bCreateSessionSuccecssful)
	{
		UE_LOG(LogTemp, Display, TEXT("ASTPlayerController : Session Created Successfully."));
	}
}

void ASTPlayerController::FindSessionInfos()
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid()) return;

	Handle_FindSessionComplete =
		sessionInterface->AddOnFindSessionsCompleteDelegate_Handle(Delegate_FindSessionComplete);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = 100;
	LastSessionSearch->bIsLanQuery = true;

	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->FindSessions(*localPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(Handle_FindSessionComplete);
	}
}

void ASTPlayerController::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (!bWasSuccessful) return;

	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid()) return;

	if (sessionInterface)
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(Handle_FindSessionComplete);
	}

	SessionInfos.Empty();
	for (auto& result : LastSessionSearch->SearchResults)
	{
		FSessionInfo info;

		// Session Name
		FString sessionString;
		result.Session.SessionSettings.Get(FName("SessionName"), sessionString);

		info.SessionName = UKismetStringLibrary::Conv_StringToName(sessionString);
		info.SearchResult = result;

		SessionInfos.Push(info);
	}

	for (auto& info : SessionInfos)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Session Founded!!")));
		UE_LOG(LogTemp, Display, TEXT("Session Detected : %s"), *info.SessionName.ToString());
	}
}

void ASTPlayerController::OnRep_SessionInfos()
{
	for (auto& info : SessionInfos)
	{
		UE_LOG(LogTemp, Display, TEXT("Session Detected : %s"), *info.SessionName.ToString());
	}
}

#pragma endregion