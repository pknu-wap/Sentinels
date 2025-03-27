// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UI/STPlayerUIComponent.h"
#include "Net/UnrealNetwork.h"
#include "Subsystem/STUISubSystem.h"
#include "STGameplayTags.h"
#include "Sentinels_LS.h"
#include "EngineUtils.h"
#include "Player/STPlayerController.h"

USTPlayerUIComponent::USTPlayerUIComponent()
	:bIsReady(false)
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
}

void USTPlayerUIComponent::ServerRPCSetbIsReady_Implementation(bool Value)
{
	bIsReady = Value;
}

void USTPlayerUIComponent::ServerRPCChangebIsReady_Implementation()
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
			playerController->GetUIComponent()->ClientRPCCheckbIsReady();
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("All Ready"));
}


void USTPlayerUIComponent::ClientRPCCheckbIsReady_Implementation()
{
	USTUISubSystem* UISubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USTUISubSystem>();
	UISubSystem->UnRegisterWidget(FGameplayTag::RequestGameplayTag(FName("Widget.Lobby.Ready")));
}
