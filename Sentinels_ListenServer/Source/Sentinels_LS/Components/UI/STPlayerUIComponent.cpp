// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UI/STPlayerUIComponent.h"
#include "Net/UnrealNetwork.h"
#include "Subsystem/STUISubSystem.h"
#include "STGameplayTags.h"
#include "Sentinels_LS.h"
#include "EngineUtils.h"
#include "Player/STPlayerController.h"
#include "GameFramework/PlayerState.h"
//
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Player/Dummy/STDummyPlayer.h"
#include "Engine/TextureRenderTarget2D.h"

USTPlayerUIComponent::USTPlayerUIComponent()
	:bIsReady(false),
	STClassType(ESTClassType::GreatSword)
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
	DOREPLIFETIME(USTPlayerUIComponent, STClassType);
	DOREPLIFETIME(USTPlayerUIComponent, PlayerID);
}

void USTPlayerUIComponent::ServerRPCSetbIsReady_Implementation(bool Value)
{
	bIsReady = Value;
}

void USTPlayerUIComponent::ServerRPCChangebIsReady_Implementation(FGameplayTag WidgetTag)
{
	for (ASTPlayerController* playerController : TActorRange<ASTPlayerController>(GetWorld()))
	{
		if (playerController)
		{
			if (!playerController->GetUIComponent()->GetbIsReady())
			{
				UE_LOG(LogTemp, Warning, TEXT("Not Ready"));
				return;
			}
		}
	}

	for (ASTPlayerController* playerController : TActorRange<ASTPlayerController>(GetWorld()))
	{
		if (playerController)
		{
			playerController->GetUIComponent()->ClientRPCUnRegisterWidget(WidgetTag);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("All Ready"));
}

void USTPlayerUIComponent::ClientRPCUnRegisterWidget_Implementation(FGameplayTag WidgetTag)
{
	USTUISubSystem* UISubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();
	UISubSystem->UnRegisterWidget(WidgetTag);
}

void USTPlayerUIComponent::ServerRPCRegisterPlayerID_Implementation(const FUniqueNetIdRepl& ID)
{
	ST_SUBLOG(LogSTNetwork, Warning, TEXT("Begin"));

	ASTPlayerController* serverPC = Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	if (ID != serverPC->PlayerState->GetUniqueId() && !serverPC->GetUIComponent()->PlayerID.Contains(ID))
	{
		ST_SUBLOG(LogSTNetwork, Warning, TEXT("ADD"));
		serverPC->GetUIComponent()->PlayerID.Add(ID);
		// 서버가 아닌 것이 들어올 때, 여기서 더미 플레이어를 일단 추가하는 방식으로
	}

	for (ASTPlayerController* playerController : TActorRange<ASTPlayerController>(GetWorld()))
	{
		if (playerController == serverPC)
			continue;

		for (auto iter : serverPC->GetUIComponent()->PlayerID)
		{
			if (playerController->PlayerState->GetUniqueId() != iter && !playerController->GetUIComponent()->PlayerID.Contains(iter))
			{
				playerController->GetUIComponent()->PlayerID.Add(iter);
				//playerController->GetUIComponent()->RegisterIDToDummyPlayer(ID);
				//playerController->GetUIComponent()->UpdatePlayerID();
			}
				
		}
	}

	// 더미 플레이어에게 PlayerID 부착 (서버 포함)
	//RegisterIDToDummyPlayer(ID);

	//UpdatePlayerID();
}

void USTPlayerUIComponent::ServerRPCUnRegisterPlayerID_Implementation(const FUniqueNetIdRepl& ID)
{
	for (ASTPlayerController* playerController : TActorRange<ASTPlayerController>(GetWorld()))
	{
		if (playerController)
		{
			USTPlayerUIComponent* uIComponent = playerController->GetUIComponent();

			for (auto iter : PlayerID)
			{
				if (!uIComponent->PlayerID.Contains(iter))
				{
					ST_SUBLOG(LogSTNetwork, Warning, TEXT("Add ID"));
					uIComponent->PlayerID.Add(iter);
				}
			}

		}
	}
}

void USTPlayerUIComponent::UpdatePlayerID()
{
	USTUISubSystem* UISubSystem = GetWorld()->GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();
	FGameplayTag widgetTag = FGameplayTag::RequestGameplayTag(FName("Widget.Lobby.Ready"));
	UUserWidget* widgetInstance = UISubSystem->GetWidget(widgetTag);

	if (!widgetInstance)
		return;

	UImage* img_FirstPlayer = Cast<UImage>(widgetInstance->GetWidgetFromName(TEXT("IMG_FirstPlayer")));
	UImage* img_SecondPlayer = Cast<UImage>(widgetInstance->GetWidgetFromName(TEXT("IMG_SecondPlayer")));
	UImage* img_ThirdPlayer = Cast<UImage>(widgetInstance->GetWidgetFromName(TEXT("IMG_ThirdPlayer")));

	if (!img_FirstPlayer || !img_SecondPlayer || !img_ThirdPlayer)
		return;

	APlayerController* pc = GetWorld()->GetFirstPlayerController();

	if (!pc)
		return;

	for (auto iter : PlayerID)
	{
		if (iter == pc->PlayerState->GetUniqueId())
		{
			continue;
		}
		else
		{
			ASTDummyPlayer* dummyPlayer = ASTDummyPlayer::FindByID(GetWorld(), iter);
			UTextureRenderTarget2D* renderTarget2D = dummyPlayer->GetTextureRenderTarget2D();

			FSlateBrush slateBrush;
			slateBrush.SetResourceObject(renderTarget2D);
			slateBrush.ImageSize = FVector2D(renderTarget2D->SizeX, renderTarget2D->SizeY);
			slateBrush.DrawAs = ESlateBrushDrawType::Image;
		}
	}
}

void USTPlayerUIComponent::OnRep_PlayerID()
{
	//UpdatePlayerID();
}

void USTPlayerUIComponent::RegisterIDToDummyPlayer(const FUniqueNetIdRepl& ID)
{
	for (ASTDummyPlayer* dummyPlayer : TActorRange<ASTDummyPlayer>(GetWorld()))
	{
		if (dummyPlayer->GetPlayerID() != nullptr)
			continue;

		dummyPlayer->SetPlayerID(ID);
	}
}

