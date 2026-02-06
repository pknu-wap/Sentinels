// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/STPlayerController.h"
#include "Components/SkillComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/InteractComponent.h"
#include "Components/SplineComponent.h"
#include "EnhancedInputComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Sentinels_LSGameMode.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSubsystemTypes.h"
#include "OnlineSubsystemUtils.h"
#include "GameFramework/PlayerState.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "Components/UI/STPlayerUIComponent.h"
#include "SubSystem/STGameTravelDataSubsystem.h"
#include "Player/STLocalPlayer.h"
#include "SubSystem/STWorldSpawnSubsystem.h"
#include "System/STGameState.h"
#include "SubSystem/STUISubSystem.h"
#include "Player/STPlayerState.h"

ASTPlayerController::ASTPlayerController()
{
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComp"));
	SkillComponent->SetIsReplicated(true);

	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComp"));
	InteractComponent->SetIsReplicated(true);

	UIComponent = CreateDefaultSubobject<USTPlayerUIComponent>(TEXT("UIComponent"));
	UIComponent->SetIsReplicated(true);
}

void ASTPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if(InteractComponent)
		InteractComponent->BindDelegates();
}

void ASTPlayerController::BeginPlay()
{
	Super::BeginPlay();

	USTWorldSpawnSubsystem* WorldSpawnSystem = GetWorld()->GetSubsystem<USTWorldSpawnSubsystem>();
	if (WorldSpawnSystem)
	{
		WorldSpawnSystem->PlayerNumUpdated(GetWorld()->GetNumPlayerControllers());
	}

	/*USTLocalPlayer* LocalPlayer = Cast<USTLocalPlayer>(GetLocalPlayer());
	if (LocalPlayer && !(LocalPlayer->bIsRegistered))
	{
		RegisterSelfToSession(LocalPlayer->GetSessionName());
		LocalPlayer->bIsRegistered = true;
	}*/	
}

void ASTPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
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

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASTPlayerController::Interact_Pressed);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ASTPlayerController::Interact_Released);

		EnhancedInputComponent->BindAction(ShowTeamInfoUIAction, ETriggerEvent::Started, this, &ASTPlayerController::ShowTeamInfoUI_Pressed);
		EnhancedInputComponent->BindAction(ShowTeamInfoUIAction, ETriggerEvent::Completed, this, &ASTPlayerController::ShowTeamInfoUI_Released);
		EnhancedInputComponent->BindAction(ShowMapUIAction, ETriggerEvent::Completed, this, &ASTPlayerController::ShowMapUI_Pressed);
		EnhancedInputComponent->BindAction(ShowSystemUIAction, ETriggerEvent::Completed, this, &ASTPlayerController::ShowSystemUI_Pressed);
	}
}

void ASTPlayerController::BindDefaultTopDownInput()
{
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

	SetShowMouseCursor(true);
}

void ASTPlayerController::BindDefaultThirdPersonInput()
{
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	SetShowMouseCursor(false);
}

void ASTPlayerController::UpdatePlayerClass_Implementation(ESTClassType InClass)
{
	GetPawn()->Destroy();

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	if (!GameMode) return;
	 
	switch (InClass)
	{
	case ESTClassType::GreatSword:
		DefaultPlayerClass = PawnClass_GreatSword;
		break;
	case ESTClassType::Katana:
		DefaultPlayerClass = PawnClass_Katana;
		break;
	case ESTClassType::DualBlade:
		DefaultPlayerClass = PawnClass_DualBlade;
		break;
	case ESTClassType::Magician:
		DefaultPlayerClass = PawnClass_Magician;
		break;
	default:
		break;
	}

	if (ASTPlayerState* PS = Cast<ASTPlayerState>(PlayerState))
	{
		PS->UpdatePlayerClass(InClass);
	}

	AController* PC = this;
	GetWorldTimerManager().SetTimerForNextTick([GameMode, PC]() { GameMode->RestartPlayer(PC); });
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
				// DrawDebugSphere(GetWorld(), point, 10.f, 8, FColor::Green, false, 5.f);
			}

			GetWorldTimerManager().SetTimer(Handle_AutoRun, this, &ASTPlayerController::AutoRun, GetWorld()->GetDeltaSeconds(), true);
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

void ASTPlayerController::ShowTeamInfoUI_Pressed()
{
	USTUISubSystem* UISubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();
	UWidget* widget = UISubSystem->GetWidget(FSTGameplayTags::Get().Widget_InGame_TeamInfo);

	if (widget)
	{
		widget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void ASTPlayerController::ShowTeamInfoUI_Released()
{
	USTUISubSystem* UISubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();
	UWidget* widget = UISubSystem->GetWidget(FSTGameplayTags::Get().Widget_InGame_TeamInfo);

	if (widget)
	{
		widget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ASTPlayerController::ShowMapUI_Pressed()
{
	USTUISubSystem* UISubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();
	UWidget* miniMapWidget = UISubSystem->GetWidget(FSTGameplayTags::Get().Widget_InGame_Minimap);
	UWidget* gameMapWidget = UISubSystem->GetWidget(FSTGameplayTags::Get().Widget_InGame_GameMap);

	if (!miniMapWidget || !gameMapWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error : miniMap & gameMap Widget don't exist"));
		return;
	}

	if (gameMapWidget->GetVisibility() == ESlateVisibility::Collapsed)
	{
		gameMapWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		UIComponent->UpdateUI(FSTGameplayTags::Get().Widget_InGame_GameMap);
	}
	else
	{
		gameMapWidget->SetVisibility(ESlateVisibility::Collapsed);
		UIComponent->UpdateUI(FSTGameplayTags::Get().Widget_InGame_Minimap);
	}
}

void ASTPlayerController::ShowSystemUI_Pressed()
{
	USTUISubSystem* UISubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();
	UUserWidget* systemWidget = UISubSystem->GetWidget(FSTGameplayTags::Get().Widget_OutGame_System);

	if (!systemWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error : systemWidget don't exist"));
		return;
	}

	if (!systemWidget->IsInViewport())
	{
		systemWidget->AddToViewport();
		SetShowMouseCursor(true);
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
		systemWidget->RemoveFromParent();
		SetShowMouseCursor(false);
	}
}

void ASTPlayerController::Interact_Pressed()
{
	if(InteractComponent)
		InteractComponent->Interact_Server();
}

void ASTPlayerController::Interact_Released()
{
	if (InteractComponent)
		InteractComponent->Interact_Finish_Server();
}

void ASTPlayerController::RegisterSelfToSession_Client_Implementation()
{
	USTLocalPlayer* localPlayer = Cast<USTLocalPlayer>(GetLocalPlayer());

	if (!localPlayer || localPlayer->GetSessionName() == FName()) return;

	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		
		if (localPlayer)
		{
			if (!sessionInterface->RegisterPlayer(localPlayer->GetSessionName(), *localPlayer->GetPreferredUniqueNetId(), false))
			{
				UE_LOG(LogTemp, Warning, TEXT("Client : ASTPlayerController Failed To Register Player To Session."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Client : ASTPlayerController Success To Register Player To Session."));
			}
		}
	}

	RegisterSelfToSession_Server(localPlayer->GetSessionName());
}

void ASTPlayerController::ServerRPCRegisterPlayerInfo_Implementation(FPlayerInfo PlayerInfo)
{
	//USTGameInstance* gameInstance = Cast<USTGameInstance>(GetWorld()->GetGameInstance());

	FUniqueNetIdRepl playerID = PlayerState->GetUniqueId();

	//if (gameInstance->PlayerInfos.Find(playerID) != nullptr || !playerID.IsValid())
	//{
	//	gameInstance->PlayerInfos.Find(playerID)->PlayerClass = PlayerInfo.PlayerClass;
	//}
	//else
	//{
	//	ESTClassType playerClass = ESTClassType::GreatSword;
	//	gameInstance->PlayerInfos.Add(playerID, {playerClass});
	//}
}

void ASTPlayerController::RegisterSelfToSession_Server_Implementation(FName SessionName)
{
	UWorld* world = GetWorld();
	if (!world || SessionName == FName()) return;

	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		if (USTLocalPlayer* localPlayer = Cast<USTLocalPlayer>(GetLocalPlayer()))
		{
			if (!sessionInterface->RegisterPlayer(SessionName, *localPlayer->GetPreferredUniqueNetId(), false))
			{
				UE_LOG(LogTemp, Warning, TEXT("Client : ASTPlayerController Failed To Register Player To Session."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Client : ASTPlayerController Success To Register Player To Session."));
			}
		}
		else if(PlayerState)
		{
			if (!sessionInterface->RegisterPlayer(SessionName, *PlayerState->GetUniqueId(), false))
			{
				UE_LOG(LogTemp, Warning, TEXT("Client : ASTPlayerController Failed To Register Player To Session."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Client : ASTPlayerController Success To Register Player To Session."));
			}
		}
	}
}

