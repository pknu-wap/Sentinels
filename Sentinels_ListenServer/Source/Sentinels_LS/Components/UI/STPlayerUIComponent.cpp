// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UI/STPlayerUIComponent.h"
#include "Net/UnrealNetwork.h"
#include "Subsystem/STUISubSystem.h"
#include "Sentinels_LS.h"
#include "EngineUtils.h"
#include "Player/STPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "STGameplayTags.h"
#include "GameplayTagsManager.h"
#include "System/STGameState.h"
#include "System/STGameInstance.h"
#include "Player/Dummy/STDummyPlayer.h"
#include "Components/Image.h"
#include "Materials/Material.h"
#include "Engine/TextureRenderTarget2D.h"

USTPlayerUIComponent::USTPlayerUIComponent() :
	bIsReady(false)
{
}

void USTPlayerUIComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USTPlayerUIComponent::InitializeComponent()
{

}

void USTPlayerUIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USTPlayerUIComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USTPlayerUIComponent, bIsReady);
	DOREPLIFETIME(USTPlayerUIComponent, PlayerID);
}

const FGameplayTag USTPlayerUIComponent::GetCurrentLevelTag()
{
	ASTGameState* gameState = GetWorld()->GetGameState<ASTGameState>();
	return gameState->GetCurrentLevelTag();
}

void USTPlayerUIComponent::SetCurrentLevelTag(FGameplayTag LevelTag)
{
	for (ASTPlayerController* playerController : TActorRange<ASTPlayerController>(GetWorld()))
	{
		if (playerController)
		{
			if (playerController->HasAuthority())
			{
				ASTGameState* gameState = GetWorld()->GetGameState<ASTGameState>();
				gameState->SetCurrentLevelTag(LevelTag);
				Cast<USTGameInstance>(playerController->GetGameInstance())->CurrentLevelTag = LevelTag;
			}
		}
	}
}

void USTPlayerUIComponent::ClientRPCRegisterWidget_Implementation(FGameplayTag WidgetTag, TSubclassOf<UUserWidget> WidgetClass)
{
	USTUISubSystem* UISubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();

	ASTPlayerController* pc = Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	UUserWidget* widget = UISubSystem->GetWidget(WidgetTag);
	if (!widget)
	{
		widget = CreateWidget<UUserWidget>(pc, WidgetClass);
		UISubSystem->RegisterWidget(WidgetTag, widget);
	}
}

void USTPlayerUIComponent::ClientRPCUnRegisterWidget_Implementation(FGameplayTag WidgetTag)
{
	USTUISubSystem* UISubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();
	UISubSystem->UnRegisterWidget(WidgetTag);
}

void USTPlayerUIComponent::ClientRPCAddToViewport_Implementation(FGameplayTag WidgetTag)
{
	USTUISubSystem* UISubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();

	UUserWidget* widget = UISubSystem->GetWidget(WidgetTag);
	if (!widget)
	{
		ST_SUBLOG(LogSTNetwork, Warning, TEXT("No Widget"));
		return;
	}

	widget->AddToViewport();
}

void USTPlayerUIComponent::ClientRPCRemoveFromParent_Implementation(FGameplayTag WidgetTag)
{
	USTUISubSystem* UISubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();

	UUserWidget* widget = UISubSystem->GetWidget(WidgetTag);
	if (!widget)
	{
		ST_SUBLOG(LogSTNetwork, Warning, TEXT("No Widget"));
		return;
	}

	widget->RemoveFromParent();
}

void USTPlayerUIComponent::ClientRPCUpdateUI_Implementation(FGameplayTag WidgetTag)
{
	if (FSTGameplayTags::Get().Widget_Lobby_Loadout == WidgetTag)
	{
		UpdateLoadoutUI();
		UpdateCurrentGameLevelUI();
	}
}


void USTPlayerUIComponent::ServerRPCUpdateUI_Implementation(FGameplayTag WidgetTag)
{
	for (ASTPlayerController* playerController : TActorRange<ASTPlayerController>(GetWorld()))
	{
		if (playerController)
		{
			playerController->GetUIComponent()->ClientRPCUpdateUI(WidgetTag);
		}
	}
}

void USTPlayerUIComponent::ServerRPCRegisterPlayerID_Implementation(const FUniqueNetIdRepl& ID)
{
	AddPlayerID(ID);

	ServerRPCUpdateUI(FSTGameplayTags::Get().Widget_Lobby_Loadout);

	UpdateLoadoutUI();
}

void USTPlayerUIComponent::ServerRPCUnRegisterPlayerID_Implementation(const FUniqueNetIdRepl& ID)
{

}

void USTPlayerUIComponent::ServerRPCSetbIsReady_Implementation(bool Value)
{
	bIsReady = Value;
}

void USTPlayerUIComponent::ServerRPCCheckbIsReady_Implementation(FGameplayTag WidgetTag)
{
	for (ASTPlayerController* playerController : TActorRange<ASTPlayerController>(GetWorld()))
	{
		if (playerController)
		{
			if (!playerController->GetUIComponent()->GetbIsReady())
			{
				ST_SUBLOG(LogSTNetwork, Warning, TEXT("Not Ready"));
				return;
			}
		}
	}

	if (!GetWorld()->GetGameState<ASTGameState>()->CanServerTravel())
		return;

	for (ASTPlayerController* playerController : TActorRange<ASTPlayerController>(GetWorld()))
	{
		if (playerController)
		{
			playerController->GetUIComponent()->ClientRPCRemoveFromParent(WidgetTag);
		}
	}

	GetWorld()->GetGameState<ASTGameState>()->TryServerTravel();
}

void USTPlayerUIComponent::UpdateLoadoutUI()
{
	USTUISubSystem* UISubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();

	ASTPlayerController* pc = Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!pc)
	{
		return;
	}

	for (auto playerIDIter : pc->GetUIComponent()->PlayerID)
	{
		if (playerIDIter == pc->PlayerState->GetUniqueId())
		{
			continue;
		}
		else
		{
			ASTDummyPlayer* dummyPlayer = nullptr;
			for (ASTDummyPlayer* dummyPlayerIter : TActorRange<ASTDummyPlayer>(GetWorld()))
			{
				if (dummyPlayerIter->GetPlayerID().ToString() == playerIDIter.ToString())
					dummyPlayer = dummyPlayerIter;
			}

			if (!dummyPlayer)
			{
				ST_SUBLOG(LogSTNetwork, Warning, TEXT("dummyPlayer not found"));
				return;
			}

			if (dummyPlayer->GetbIsShow())
				continue;

			UMaterial* material = dummyPlayer->GetMaterial();
			if (!material)
			{
				ST_SUBLOG(LogSTNetwork, Warning, TEXT("Can't find Material!"));
				return;
			}

			FGameplayTag widgetTag = FSTGameplayTags::Get().Widget_Lobby_Loadout;
			UUserWidget* widgetInstance = UISubSystem->GetWidget(widgetTag);
			if (!widgetInstance)
			{
				ST_SUBLOG(LogSTNetwork, Warning, TEXT("Can't find widgetInstance!"));
				return;
			}

			UImage* img_FirstPlayer = Cast<UImage>(widgetInstance->GetWidgetFromName(TEXT("IMG_FirstPlayer")));
			UImage* img_SecondPlayer = Cast<UImage>(widgetInstance->GetWidgetFromName(TEXT("IMG_SecondPlayer")));
			UImage* img_ThirdPlayer = Cast<UImage>(widgetInstance->GetWidgetFromName(TEXT("IMG_ThirdPlayer")));

			if (img_FirstPlayer->GetBrush().GetResourceObject() == nullptr)
			{
				img_FirstPlayer->SetVisibility(ESlateVisibility::Visible);
				img_FirstPlayer->SetBrushFromMaterial(material);
				dummyPlayer->SetbIsShow(true);
			}
			else if (img_SecondPlayer->GetBrush().GetResourceObject() == nullptr)
			{
				img_SecondPlayer->SetVisibility(ESlateVisibility::Visible);
				img_SecondPlayer->SetBrushFromMaterial(material);
				dummyPlayer->SetbIsShow(true);
			}
			else if (img_ThirdPlayer->GetBrush().GetResourceObject() == nullptr)
			{
				img_ThirdPlayer->SetVisibility(ESlateVisibility::Visible);
				img_ThirdPlayer->SetBrushFromMaterial(material);
				dummyPlayer->SetbIsShow(true);
			}		
		}
	}
}

void USTPlayerUIComponent::UpdateCurrentGameLevelUI()
{
	FGameplayTag currentLevelTag = GetCurrentLevelTag();

	if (currentLevelTag == FSTGameplayTags::Get().Level_Lobby)
		return;

	USTUISubSystem* UISubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();

	ASTPlayerController* pc = Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!pc)
	{
		return;
	}

	FGameplayTag loadoutTag = FSTGameplayTags::Get().Widget_Lobby_Loadout;
	FGameplayTag selectLevelTag = FSTGameplayTags::Get().Widget_Lobby_LevelSelect;

	UUserWidget* loadoutWidgetInstance = UISubSystem->GetWidget(loadoutTag);
	UUserWidget* selectLevelWidgetInstance = UISubSystem->GetWidget(selectLevelTag);
	if (!loadoutWidgetInstance || !selectLevelWidgetInstance)
	{
		ST_SUBLOG(LogSTNetwork, Warning, TEXT("NO loadoutWidgetInstance or NO selectLevelWidgetInstance"));
		return;
	}

	FString cleanTagName = currentLevelTag.ToString().Replace(TEXT("."), TEXT("_"));
	FString widgetNameStr = FString::Printf(TEXT("IMG_%s"), *cleanTagName);

	const FSlateBrush& sourceBrush = Cast<UImage>(selectLevelWidgetInstance->GetWidgetFromName(*widgetNameStr))->GetBrush();
	FSlateBrush copiedBrush = sourceBrush;

	UImage* img_CurrentLevel = Cast<UImage>(loadoutWidgetInstance->GetWidgetFromName(TEXT("IMG_CurrentLevel")));
	img_CurrentLevel->SetBrush(copiedBrush);
	img_CurrentLevel->SetVisibility(ESlateVisibility::Visible);
}

void USTPlayerUIComponent::UpdateCharacterSelectUI()
{
	USTUISubSystem* UISubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();
	if (!UISubSystem)
	{
		ST_SUBLOG(LogSTNetwork, Warning, TEXT("Can't find UISubSystem!"));
		return;
	}

	ASTPlayerController* pc = Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!pc || !pc->PlayerState)
	{
		ST_SUBLOG(LogSTNetwork, Warning, TEXT("Invalid PlayerController or PlayerState"));
		return;
	}
	else if (!pc->IsLocalPlayerController())
	{
		ST_SUBLOG(LogSTNetwork, Warning, TEXT("No Local Player"));
		return;
	}

	ASTDummyPlayer* dummyPlayer = nullptr;
	for (ASTDummyPlayer* dummyPlayerIter : TActorRange<ASTDummyPlayer>(GetWorld()))
	{
		if (dummyPlayerIter->GetPlayerID().ToString() == pc->PlayerState->GetUniqueId().ToString())
			dummyPlayer = dummyPlayerIter;
	}

	if (!dummyPlayer)
	{
		ST_SUBLOG(LogSTNetwork, Warning, TEXT("dummyPlayer not found"));
		return;
	}

	if (dummyPlayer->GetbIsShow())
	{
		ST_SUBLOG(LogSTNetwork, Warning, TEXT("Alreay show"));
		return;
	}

	UMaterial* material = dummyPlayer->GetMaterial();
	if (!material)
	{
		ST_SUBLOG(LogSTNetwork, Warning, TEXT("Can't find Material!"));
		return;
	}

	FGameplayTag widgetTag = FSTGameplayTags::Get().Widget_Lobby_CharacterSelect;

	UUserWidget* widgetInstance = UISubSystem->GetWidget(widgetTag);
	if (!widgetInstance)
	{
		ST_SUBLOG(LogSTNetwork, Warning, TEXT("NO WIDGET INSTANCE"));
		return;
	}

	UImage* img_LocalPlayer = Cast<UImage>(widgetInstance->GetWidgetFromName(TEXT("IMG_LocalPlayer")));
	if (!img_LocalPlayer)
	{
		ST_SUBLOG(LogSTNetwork, Warning, TEXT("no img_LocalPlayer"));
		return;
	}

	if (img_LocalPlayer->GetBrush().GetResourceObject() == nullptr)
	{
		img_LocalPlayer->SetVisibility(ESlateVisibility::Visible);
		img_LocalPlayer->SetBrushFromMaterial(material);
		dummyPlayer->SetbIsShow(true);
	}
}

void USTPlayerUIComponent::AddPlayerID(const FUniqueNetIdRepl& ID)
{
	ASTPlayerController* serverPC = Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	if (!serverPC->GetUIComponent()->PlayerID.Contains(ID))
	{
		ST_SUBLOG(LogSTNetwork, Warning, TEXT("ADD"));
		serverPC->GetUIComponent()->PlayerID.Add(ID);
	}

	for (ASTPlayerController* playerController : TActorRange<ASTPlayerController>(GetWorld()))
	{
		if (playerController == serverPC)
			continue;

		for (auto iter : serverPC->GetUIComponent()->PlayerID)
		{
			if (!playerController->GetUIComponent()->PlayerID.Contains(iter))
			{
				playerController->GetUIComponent()->PlayerID.Add(iter);
			}

		}
	}
		
	RegisterIDToDummyPlayer(ID);
}

void USTPlayerUIComponent::RegisterIDToDummyPlayer(const FUniqueNetIdRepl& ID)
{
	for (ASTDummyPlayer* dummyPlayer : TActorRange<ASTDummyPlayer>(GetWorld()))
	{
		if (dummyPlayer->GetPlayerID() == ID)
			return;

		if (dummyPlayer->GetPlayerID().IsValid())
			continue;
		
		dummyPlayer->SetPlayerID(ID);

		for (ASTPlayerController* playerController : TActorRange<ASTPlayerController>(GetWorld()))
		{
			if (playerController->PlayerState->GetUniqueId() == ID)
			{
				dummyPlayer->SetOwner(playerController);
			}
		}

		break;
	}
}

