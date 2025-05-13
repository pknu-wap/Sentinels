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
#include "Player/Dummy/STDummyCharacter.h"
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

void USTPlayerUIComponent::ServerRPCRegisterPlayerID_Implementation(const FUniqueNetIdRepl& ID)
{
	AddPlayerID(ID);
}

void USTPlayerUIComponent::ServerRPCRegisterWidget_Implementation(FGameplayTag WidgetTag, TSubclassOf<UUserWidget> WidgetClass)
{
	ASTPlayerController* serverPC = Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (serverPC)
		serverPC->GetUIComponent()->RegisterWidget(WidgetTag, WidgetClass);

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
	RegisterWidget(WidgetTag, WidgetClass);
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

	UImage* img_CurrentLevel = Cast<UImage>(loadoutWidgetInstance->GetWidgetFromName(TEXT("IMG_CurrentLevel")));

	if (img_CurrentLevel->GetBrush().GetResourceObject() == nullptr)
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
		UpdatePlayerWeaponLayer();
		UpdateCurrentGameLevelLayer();
	}
}

void USTPlayerUIComponent::UpdatePlayerWeaponLayer()
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
		ASTDummyCharacter* dummyCharacter = nullptr;
		for (ASTDummyCharacter* dummyCharacterIter : TActorRange<ASTDummyCharacter>(GetWorld()))
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

		if (playerIDIter == pc->PlayerState->GetUniqueId())
		{
			UImage* img_LocalPlayer = Cast<UImage>(widgetInstance->GetWidgetFromName(TEXT("IMG_Self")));
			if (!img_LocalPlayer)
			{
				ST_SUBLOG(LogSTNetwork, Warning, TEXT("no IMG_Self"));
				return;
			}

			if (img_LocalPlayer->GetBrush().GetResourceObject() == nullptr)
			{
				img_LocalPlayer->SetVisibility(ESlateVisibility::Visible);
				img_LocalPlayer->SetBrushFromMaterial(material);
				dummyCharacter->SetbIsShow(true);
			}
		}
		else
		{
			UImage* img_FirstPlayer = Cast<UImage>(widgetInstance->GetWidgetFromName(TEXT("IMG_FirstPlayer")));
			UImage* img_SecondPlayer = Cast<UImage>(widgetInstance->GetWidgetFromName(TEXT("IMG_SecondPlayer")));
			UImage* img_ThirdPlayer = Cast<UImage>(widgetInstance->GetWidgetFromName(TEXT("IMG_ThirdPlayer")));

			if (img_FirstPlayer->GetBrush().GetResourceObject() == nullptr)
			{
				img_FirstPlayer->SetVisibility(ESlateVisibility::Visible);
				img_FirstPlayer->SetBrushFromMaterial(material);
				dummyCharacter->SetbIsShow(true);
			}
			else if (img_SecondPlayer->GetBrush().GetResourceObject() == nullptr)
			{
				img_SecondPlayer->SetVisibility(ESlateVisibility::Visible);
				img_SecondPlayer->SetBrushFromMaterial(material);
				dummyCharacter->SetbIsShow(true);
			}
			else if (img_ThirdPlayer->GetBrush().GetResourceObject() == nullptr)
			{
				img_ThirdPlayer->SetVisibility(ESlateVisibility::Visible);
				img_ThirdPlayer->SetBrushFromMaterial(material);
				dummyCharacter->SetbIsShow(true);
			}		
		}
	}
}

void USTPlayerUIComponent::UpdateCurrentGameLevelLayer()
{
	FGameplayTag currentLevelTag = GetWorld()->GetGameState<ASTGameState>()->GetCurrentLevelTag();

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

	UTextBlock* tb_LevelName = Cast<UTextBlock>(loadoutWidgetInstance->GetWidgetFromName(TEXT("TB_LevelName")));
	tb_LevelName->SetText(FText::FromString(cleanTagName));
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

	ASTDummyCharacter* dummyCharacter = nullptr;
	for (ASTDummyCharacter* dummyCharacterIter : TActorRange<ASTDummyCharacter>(GetWorld()))
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

	FGameplayTag widgetTag = FSTGameplayTags::Get().Widget_Lobby_CharacterSelect;

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

void USTPlayerUIComponent::RegisterWidget(FGameplayTag WidgetTag, TSubclassOf<UUserWidget> WidgetClass)
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
	for (ASTDummyCharacter* dummyCharacter : TActorRange<ASTDummyCharacter>(GetWorld()))
	{
		if (dummyCharacter->GetPlayerID() == ID)
			return;

		if (dummyCharacter->GetPlayerID().IsValid())
			continue;
		
		dummyCharacter->SetPlayerID(ID);

		for (ASTPlayerController* playerController : TActorRange<ASTPlayerController>(GetWorld()))
		{
			if (playerController->PlayerState->GetUniqueId() == ID)
			{
				dummyCharacter->SetOwner(playerController);
			}
		}

		break;
	}
}
