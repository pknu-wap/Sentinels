// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interact/STDimensionDrift.h"
#include "Net/UnrealNetwork.h"
#include "Components/SkeletalMeshComponent.h"
#include "Subsystem/STUISubSystem.h"
#include "Player/STPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "STGameplayTags.h"
#include "Components/InteractComponent.h"
#include "Components/UI/STPlayerUIComponent.h"
#include "GameFramework/PlayerState.h"
#include "Engine/Level.h"
#include "Misc/PackageName.h"
#include "Sentinels_LS.h"
#include "System/STGameState.h"
#include "Player/Dummy/STDummyPlayer.h"
#include "EngineUtils.h"

ASTDimensionDrift::ASTDimensionDrift()
{
	PrimaryActorTick.bCanEverTick = false;

	SKMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SKMesh;

	bReplicates = true;
}

void ASTDimensionDrift::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetGameState<ASTGameState>()->OnServerTravelReady.AddDynamic(this, &ASTDimensionDrift::HandleAllPlayerIsReady);
}

void ASTDimensionDrift::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ASTDimensionDrift::Interact_Implementation(UInteractComponent* InteractingComponent)
{
	ASTPlayerController* pc = Cast<ASTPlayerController>(InteractingComponent->GetOwner());

	USTPlayerUIComponent* uiComponent = pc->GetUIComponent();

	FGameplayTag loadoutTag = FSTGameplayTags::Get().Widget_Lobby_Loadout;
	FGameplayTag weaponSelectTag = FSTGameplayTags::Get().Widget_Lobby_WeaponSelect;
	FGameplayTag customizeTag = FSTGameplayTags::Get().Widget_Lobby_Customize;
	FGameplayTag levelSelectTag = FSTGameplayTags::Get().Widget_Lobby_LevelSelect;
	FGameplayTag loadscreenTag = FSTGameplayTags::Get().Widget_LoadScreen;

	RegisterPlayerIDToDummyPlayer(pc);

	// Server
	if (pc == Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		uiComponent->CreateAndRegisterWidget(loadoutTag, Widget_LoadoutClass);
		uiComponent->CreateAndRegisterWidget(weaponSelectTag, Widget_WeaponSelectClass);
		uiComponent->CreateAndRegisterWidget(customizeTag, Widget_CustomizeClass);
		uiComponent->CreateAndRegisterWidget(levelSelectTag, Widget_LevelSelectClass);
		uiComponent->CreateAndRegisterWidget(loadscreenTag, Widget_LoadScreen);

		uiComponent->AddToViewport(loadoutTag);
	}
	else // Client
	{
		uiComponent->ClientRPCRegisterWidget(loadoutTag, Widget_LoadoutClass);
		uiComponent->ClientRPCRegisterWidget(weaponSelectTag, Widget_WeaponSelectClass);
		uiComponent->ClientRPCRegisterWidget(customizeTag, Widget_CustomizeClass);
		uiComponent->ClientRPCRegisterWidget(levelSelectTag, Widget_LevelSelectClass);
		uiComponent->ClientRPCRegisterWidget(loadscreenTag, Widget_LoadScreen);

		uiComponent->ClientRPCAddToViewport(loadoutTag);
	}

	pc->SetInputMode(FInputModeUIOnly());
	pc->SetShowMouseCursor(true);

	// All(Local)
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, [this, loadoutTag]() {
		for (ASTPlayerController* playerController : TActorRange<ASTPlayerController>(GetWorld()))
		{
			playerController->GetUIComponent()->ClientRPCUpdateUI(loadoutTag);
		}
	}, 1.0f, false);
}

void ASTDimensionDrift::Interact_Finish_Implementation(UInteractComponent* InteractingComponent)
{
}

void ASTDimensionDrift::ShowInteractiveUI_Implementation(UInteractComponent* InteractingComponent)
{
	
}

void ASTDimensionDrift::HideInteractiveUI_Implementation(UInteractComponent* InteractingComponent)
{
}

void ASTDimensionDrift::RegisterPlayerIDToDummyPlayer(ASTPlayerController* PlayerController)
{
	for (ASTDummyPlayer* dummyPlayer : DummyPlayers)
	{
		if (dummyPlayer->GetPlayerID() == PlayerController->PlayerState->GetUniqueId())
			return;

		if (dummyPlayer->GetPlayerID().IsValid())
			continue;

		dummyPlayer->SetOwner(PlayerController);
		dummyPlayer->SetPlayerName(PlayerController->PlayerState->GetPlayerName());
		dummyPlayer->SetPlayerID(PlayerController->PlayerState->GetUniqueId());

		break;
	}
}

void ASTDimensionDrift::HandleAllPlayerIsReady(FGameplayTag NewGameLevel)
{
	GetWorld()->ServerTravel(GetLevelName(NewGameLevel));
}
