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
#include "Components/Widget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/Material.h"
#include "Engine/TextureRenderTarget2D.h"
#include "SubSystem/STGameTravelDataSubsystem.h"
#include "Player/STPlayerCharacter.h"
#include "Components/STPlayerStatusComponent.h"

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

UWidget* USTPlayerUIComponent::GetWidgetByName(UUserWidget* WidgetInstance, const FString& Name)
{
	if (!WidgetInstance) return nullptr;

	FName PropertyFName(*Name);

	UWidget* widget = Cast<UWidget>(WidgetInstance->GetWidgetFromName(PropertyFName));

	return widget;
}

void USTPlayerUIComponent::ServerRPCRegisterPlayerID_Implementation(const FUniqueNetIdRepl& ID)
{
	AddPlayerID(ID);
}

void USTPlayerUIComponent::ServerRPCRegisterWidget_Implementation(FGameplayTag WidgetTag, TSubclassOf<UUserWidget> WidgetClass)
{
	ASTPlayerController* serverPC = Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (serverPC)
		serverPC->GetUIComponent()->CreateAndRegisterWidget(WidgetTag, WidgetClass);

	for (ASTPlayerController* playerController : TActorRange<ASTPlayerController>(GetWorld()))
	{
		if (playerController && playerController != serverPC)
		{
			playerController->GetUIComponent()->ClientRPCRegisterWidget(WidgetTag, WidgetClass);
		}
	}
}

void USTPlayerUIComponent::ClientRPCRegisterWidget_Implementation(FGameplayTag WidgetTag, TSubclassOf<UUserWidget> WidgetClass)
{
	CreateAndRegisterWidget(WidgetTag, WidgetClass);
}

void USTPlayerUIComponent::ServerRPCUnRegisterPlayerID_Implementation(const FUniqueNetIdRepl& ID)
{

}

void USTPlayerUIComponent::ClientRPCUnRegisterWidget_Implementation(FGameplayTag WidgetTag)
{
	UnRegisterWidget(WidgetTag);
}

void USTPlayerUIComponent::ServerRPCUpdateUI_Implementation(FGameplayTag WidgetTag)
{
	ASTPlayerController* serverPC = Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (serverPC)
		serverPC->GetUIComponent()->UpdateUI(WidgetTag);

	for (ASTPlayerController* playerController : TActorRange<ASTPlayerController>(GetWorld()))
	{
		if (playerController && playerController != serverPC)
		{
			playerController->GetUIComponent()->ClientRPCUpdateUI(WidgetTag);
		}
	}
}

void USTPlayerUIComponent::ClientRPCUpdateUI_Implementation(FGameplayTag WidgetTag)
{
	UpdateUI(WidgetTag);
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

	USTUISubSystem* UISubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();
	FGameplayTag loadoutTag = FSTGameplayTags::Get().Widget_Lobby_Loadout;
	UUserWidget* loadoutWidgetInstance = UISubSystem->GetWidget(loadoutTag);

	FGameplayTag currentLevelTag = GetWorld()->GetGameState<ASTGameState>()->GetCurrentLevelTag();

	if (currentLevelTag == FSTGameplayTags::Get().Level_Lobby)
		return;

	for (ASTPlayerController* playerController : TActorRange<ASTPlayerController>(GetWorld()))
	{
		if (playerController)
		{
			playerController->GetUIComponent()->ClientRPCRemoveFromParent(WidgetTag);
			playerController->SetInputMode(FInputModeGameOnly());
			playerController->SetShowMouseCursor(false);
			playerController->GetUIComponent()->bIsReady = false;
		}
	}

	USTGameTravelDataSubsystem* gameTravelDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USTGameTravelDataSubsystem>();
	ASTGameState* gameState = Cast<ASTGameState>(GetWorld()->GetGameState<ASTGameState>());

	gameTravelDataSubsystem->SetCurrentLevelTag(gameState->GetCurrentLevelTag());
	gameState->TryServerTravel();
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

void USTPlayerUIComponent::UpdateUI(FGameplayTag WidgetTag)
{
	if (FSTGameplayTags::Get().Widget_Lobby_Loadout == WidgetTag)
	{
		UpdatePlayerAvatarLayer();
		UpdateCurrentGameLevelLayer();
	}
}

void USTPlayerUIComponent::UpdatePlayerAvatarLayer()
{
	USTUISubSystem* UISubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();

	ASTPlayerController* pc = Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!pc)
	{
		return;
	}

	FGameplayTag widgetTag = FSTGameplayTags::Get().Widget_Lobby_Loadout;
	UUserWidget* widgetInstance = UISubSystem->GetWidget(widgetTag);
	if (!widgetInstance)
	{
		ST_SUBLOG(LogSTNetwork, Warning, TEXT("Can't find widgetInstance!"));
		return;
	}

	for (auto playerIDIter : pc->GetUIComponent()->PlayerID)
	{
		ASTDummyPlayer* dummyCharacter = nullptr;
		for (ASTDummyPlayer* dummyCharacterIter : TActorRange<ASTDummyPlayer>(GetWorld()))
		{
			if (dummyCharacterIter->GetPlayerID().ToString() == playerIDIter.ToString())
				dummyCharacter = dummyCharacterIter;
		}

		if (!dummyCharacter)
		{
			ST_SUBLOG(LogSTNetwork, Warning, TEXT("dummyPlayer not found"));
			return;
		}

		if (dummyCharacter->GetbIsShow())
			continue;

		UMaterial* material = dummyCharacter->GetMaterial();
		if (!material)
		{
			ST_SUBLOG(LogSTNetwork, Warning, TEXT("Can't find Material!"));
			return;
		}

		UImage* img_Player1 = Cast<UImage>(widgetInstance->GetWidgetFromName(TEXT("IMG_Player1")));
		if (img_Player1 && img_Player1->GetBrush().GetResourceObject() == nullptr)
		{
			img_Player1->SetVisibility(ESlateVisibility::Visible);
			img_Player1->SetBrushFromMaterial(material);
			dummyCharacter->SetbIsShow(true);

			break;
		}

		UImage* img_Player2 = Cast<UImage>(widgetInstance->GetWidgetFromName(TEXT("IMG_Player2")));
		if (img_Player2 && img_Player2->GetBrush().GetResourceObject() == nullptr)
		{
			img_Player2->SetVisibility(ESlateVisibility::Visible);
			img_Player2->SetBrushFromMaterial(material);
			dummyCharacter->SetbIsShow(true);

			break;
		}

		UImage* img_Player3 = Cast<UImage>(widgetInstance->GetWidgetFromName(TEXT("IMG_Player3")));
		if (img_Player3 && img_Player3->GetBrush().GetResourceObject() == nullptr)
		{
			img_Player3->SetVisibility(ESlateVisibility::Visible);
			img_Player3->SetBrushFromMaterial(material);
			dummyCharacter->SetbIsShow(true);

			break;
		}

		UImage* img_Player4 = Cast<UImage>(widgetInstance->GetWidgetFromName(TEXT("IMG_Player4")));
		if (img_Player4 && img_Player4->GetBrush().GetResourceObject() == nullptr)
		{
			img_Player4->SetVisibility(ESlateVisibility::Visible);
			img_Player4->SetBrushFromMaterial(material);
			dummyCharacter->SetbIsShow(true);
		}
	}
}

void USTPlayerUIComponent::UpdateCurrentGameLevelLayer()
{
	FGameplayTag currentLevelTag = GetWorld()->GetGameState<ASTGameState>()->GetCurrentLevelTag();

	if (currentLevelTag == FSTGameplayTags::Get().Level_Lobby)
		return;

	USTUISubSystem* UISubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();

	FGameplayTag loadoutTag = FSTGameplayTags::Get().Widget_Lobby_Loadout;
	UUserWidget* loadoutWidgetInstance = UISubSystem->GetWidget(loadoutTag);
	if (!loadoutWidgetInstance)
	{
		ST_SUBLOG(LogSTNetwork, Warning, TEXT("NO loadoutWidgetInstance"));
		return;
	}

	UDataTable* mapDT = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Sentinels/UI/InGame/Map/DT_Map.DT_Map'"));

	if (!mapDT)
		return;

	const FMapInfoStruct* mapInfoRow = mapDT->FindRow<FMapInfoStruct>(*currentLevelTag.ToString(), TEXT(""));
	if (!mapInfoRow)
		return;

	UImage* img_CurrentLevel = Cast<UImage>(loadoutWidgetInstance->GetWidgetFromName(TEXT("IMG_Map")));
	FSlateBrush newBrush = img_CurrentLevel->GetBrush();
	newBrush.SetResourceObject(mapInfoRow->MapImage);
	img_CurrentLevel->SetBrush(newBrush);
	img_CurrentLevel->SetVisibility(ESlateVisibility::Visible);

	UTextBlock* tb_MapName = Cast<UTextBlock>(loadoutWidgetInstance->GetWidgetFromName(TEXT("TB_MapName")));
	UTextBlock* tb_MapDescription = Cast<UTextBlock>(loadoutWidgetInstance->GetWidgetFromName(TEXT("TB_MapDescription")));

	tb_MapName->SetText(FText::FromString(mapInfoRow->MapName));
	tb_MapDescription->SetText(FText::FromString(mapInfoRow->MapDescription));
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

	ASTDummyPlayer* dummyCharacter = nullptr;
	for (ASTDummyPlayer* dummyCharacterIter : TActorRange<ASTDummyPlayer>(GetWorld()))
	{
		if (dummyCharacterIter->GetPlayerID().ToString() == pc->PlayerState->GetUniqueId().ToString())
			dummyCharacter = dummyCharacterIter;
	}

	if (!dummyCharacter)
	{
		ST_SUBLOG(LogSTNetwork, Warning, TEXT("dummyCharacter not found"));
		return;
	}

	if (dummyCharacter->GetbIsShow())
	{
		ST_SUBLOG(LogSTNetwork, Warning, TEXT("Alreay show"));
		return;
	}

	UMaterial* material = dummyCharacter->GetMaterial();
	if (!material)
	{
		ST_SUBLOG(LogSTNetwork, Warning, TEXT("Can't find Material!"));
		return;
	}

	FGameplayTag widgetTag = FSTGameplayTags::Get().Widget_Lobby_WeaponSelect;

	UUserWidget* widgetInstance = UISubSystem->GetWidget(widgetTag);
	if (!widgetInstance)
	{
		ST_SUBLOG(LogSTNetwork, Warning, TEXT("NO WIDGET INSTANCE"));
		return;
	}

}

void USTPlayerUIComponent::UpdateMapUI()
{

}

void USTPlayerUIComponent::UpdateQuestUI()
{

}

void USTPlayerUIComponent::UpdateInventoryUI()
{

}

void USTPlayerUIComponent::UpdateTeamInfoUI()
{

}

void USTPlayerUIComponent::RegisterWidget(FGameplayTag WidgetTag, UUserWidget* Widget)
{
	USTUISubSystem* UISubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();

	ASTPlayerController* pc = Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	UUserWidget* widget = UISubSystem->GetWidget(WidgetTag);
	if (!widget)
	{
		UISubSystem->RegisterWidget(WidgetTag, Widget);
	}
}

void USTPlayerUIComponent::CreateAndRegisterWidget(FGameplayTag WidgetTag, TSubclassOf<UUserWidget> WidgetClass)
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

void USTPlayerUIComponent::UnRegisterWidget(FGameplayTag WidgetTag)
{
	USTUISubSystem* UISubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();
	UISubSystem->UnRegisterWidget(WidgetTag);
}

void USTPlayerUIComponent::AddToViewport(FGameplayTag WidgetTag)
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

void USTPlayerUIComponent::SetVisibility(FGameplayTag WidgetTag, ESlateVisibility Visibility)
{
	USTUISubSystem* UISubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();

	UUserWidget* widget = UISubSystem->GetWidget(WidgetTag);
	if (!widget)
	{
		ST_SUBLOG(LogSTNetwork, Warning, TEXT("No Widget"));
		return;
	}
	
	widget->SetVisibility(Visibility);
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
				break;
			}
		}

		break;
	}
}
