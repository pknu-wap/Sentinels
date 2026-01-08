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
#include "UI/Widget/STUserWidget.h"
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

	// if trainsition ui in viewport, delete
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
	//DOREPLIFETIME(USTPlayerUIComponent, PlayerID);
}

UWidget* USTPlayerUIComponent::GetWidgetByName(UUserWidget* WidgetInstance, const FString& Name)
{
	if (!WidgetInstance) return nullptr;

	FName PropertyFName(*Name);

	UWidget* widget = Cast<UWidget>(WidgetInstance->GetWidgetFromName(PropertyFName));

	return widget;
}

//void USTPlayerUIComponent::ServerRPCRegisterPlayerName_Implementation(const FUniqueNetIdRepl& ID)
//{
//	AddPlayerID(ID);
//}

void USTPlayerUIComponent::ServerRPCRegisterWidget_Implementation(FGameplayTag WidgetTag, TSubclassOf<UUserWidget> WidgetClass)
{
	ASTPlayerController* serverPC = Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (serverPC)
		serverPC->GetUIComponent()->CreateAndRegisterWidget(WidgetTag, WidgetClass);

	//for (ASTPlayerController* playerController : TActorRange<ASTPlayerController>(GetWorld()))
	//{
	//	if (playerController && playerController != serverPC)
	//	{
	//		playerController->GetUIComponent()->ClientRPCRegisterWidget(WidgetTag, WidgetClass);
	//	}
	//}
}

void USTPlayerUIComponent::ClientRPCRegisterWidget_Implementation(FGameplayTag WidgetTag, TSubclassOf<UUserWidget> WidgetClass)
{
	CreateAndRegisterWidget(WidgetTag, WidgetClass);
}

//void USTPlayerUIComponent::ServerRPCUnRegisterPlayerID_Implementation(const FUniqueNetIdRepl& ID)
//{
//
//}

void USTPlayerUIComponent::ClientRPCUnRegisterWidget_Implementation(FGameplayTag WidgetTag)
{
	UnRegisterWidget(WidgetTag);
}

void USTPlayerUIComponent::ServerRPCUpdateUI_Implementation(FGameplayTag WidgetTag)
{
	// RPC로 서버와 모든 클라이언트에게 업데이트 신호를 주기 위함
	// 여기서 USTUserWidget으로 캐스팅해서 캐스팅 된 애들에 한해서 업데이트 함수 호출
	ASTPlayerController* serverPC = Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (serverPC)
		UpdateUI(WidgetTag);

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

	FGameplayTag currentLevelTag = GetWorld()->GetGameState<ASTGameState>()->GetLevelTag();

	if (currentLevelTag == FSTGameplayTags::Get().Level_Lobby)
		return;

	for (ASTPlayerController* playerController : TActorRange<ASTPlayerController>(GetWorld()))
	{
		if (playerController)
		{
			playerController->GetUIComponent()->ClientRPCRemoveFromParent(WidgetTag);
			playerController->GetUIComponent()->ClientRPCAddToViewport(FSTGameplayTags::Get().Widget_LoadScreen);
			playerController->SetInputMode(FInputModeGameOnly());
			playerController->SetShowMouseCursor(false);
			playerController->GetUIComponent()->bIsReady = false;
		}
	}

	USTGameTravelDataSubsystem* gameTravelDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USTGameTravelDataSubsystem>();
	ASTGameState* gameState = Cast<ASTGameState>(GetWorld()->GetGameState<ASTGameState>());

	gameTravelDataSubsystem->SetCurrentLevelTag(gameState->GetLevelTag());
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
	USTUISubSystem* UISubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();
	USTUserWidget* widget = Cast<USTUserWidget>(UISubsystem->GetWidget(WidgetTag));

	if (widget)
		widget->UpdateUI();
}

//void USTPlayerUIComponent::UpdateCharacterSelectUI()
//{
//	USTUISubSystem* UISubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();
//	if (!UISubSystem)
//	{
//		ST_SUBLOG(LogSTNetwork, Warning, TEXT("Can't find UISubSystem!"));
//		return;
//	}
//
//	ASTPlayerController* pc = Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
//	if (!pc || !pc->PlayerState)
//	{
//		ST_SUBLOG(LogSTNetwork, Warning, TEXT("Invalid PlayerController or PlayerState"));
//		return;
//	}
//	else if (!pc->IsLocalPlayerController())
//	{
//		ST_SUBLOG(LogSTNetwork, Warning, TEXT("No Local Player"));
//		return;
//	}
//
//	ASTDummyPlayer* dummyCharacter = nullptr;
//	for (ASTDummyPlayer* dummyCharacterIter : TActorRange<ASTDummyPlayer>(GetWorld()))
//	{
//		//if (dummyCharacterIter->GetPlayerName().ToString() == pc->PlayerState->GetUniqueId().ToString())
//			//dummyCharacter = dummyCharacterIter;
//	}
//
//	if (!dummyCharacter)
//	{
//		ST_SUBLOG(LogSTNetwork, Warning, TEXT("dummyCharacter not found"));
//		return;
//	}
//
//	if (dummyCharacter->GetbIsShow())
//	{
//		ST_SUBLOG(LogSTNetwork, Warning, TEXT("Alreay show"));
//		return;
//	}
//
//	UMaterial* material = dummyCharacter->GetMaterial();
//	if (!material)
//	{
//		ST_SUBLOG(LogSTNetwork, Warning, TEXT("Can't find Material!"));
//		return;
//	}
//
//	FGameplayTag widgetTag = FSTGameplayTags::Get().Widget_Lobby_WeaponSelect;
//
//	UUserWidget* widgetInstance = UISubSystem->GetWidget(widgetTag);
//	if (!widgetInstance)
//	{
//		ST_SUBLOG(LogSTNetwork, Warning, TEXT("NO WIDGET INSTANCE"));
//		return;
//	}
//
//}

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

//void USTPlayerUIComponent::AddPlayerID(const FUniqueNetIdRepl& ID)
//{
//	ASTPlayerController* serverPC = Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
//
//	if (!serverPC->GetUIComponent()->PlayerID.Contains(ID))
//	{
//		ST_SUBLOG(LogSTNetwork, Warning, TEXT("ADD"));
//		serverPC->GetUIComponent()->PlayerID.Add(ID);
//	}
//
//	for (ASTPlayerController* playerController : TActorRange<ASTPlayerController>(GetWorld()))
//	{
//		if (playerController == serverPC)
//			continue;
//
//		for (auto iter : serverPC->GetUIComponent()->PlayerID)
//		{
//			if (!playerController->GetUIComponent()->PlayerID.Contains(iter))
//			{
//				playerController->GetUIComponent()->PlayerID.Add(iter);
//			}
//
//		}
//	}
//		
//	RegisterIDToDummyPlayer(ID);
//}
//
//void USTPlayerUIComponent::RegisterIDToDummyPlayer(const FUniqueNetIdRepl& ID)
//{
//	for (ASTDummyPlayer* dummyPlayer : TActorRange<ASTDummyPlayer>(GetWorld()))
//	{
//		if (dummyPlayer->GetPlayerName() == ID)
//			return;
//
//		if (dummyPlayer->GetPlayerName().IsValid())
//			continue;
//		
//		dummyPlayer->SetPlayerName(ID);
//
//		for (ASTPlayerController* playerController : TActorRange<ASTPlayerController>(GetWorld()))
//		{
//			if (playerController->PlayerState->GetUniqueId() == ID)
//			{
//				dummyPlayer->SetOwner(playerController);
//				break;
//			}
//		}
//
//		break;
//	}
//}

