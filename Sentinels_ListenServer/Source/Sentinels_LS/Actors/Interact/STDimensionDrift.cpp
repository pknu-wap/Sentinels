// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interact/STDimensionDrift.h"
#include "Net/UnrealNetwork.h"
#include "Components/SkeletalMeshComponent.h"
#include "Blueprint/UserWidget.h"
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

ASTDimensionDrift::ASTDimensionDrift()
{
	PrimaryActorTick.bCanEverTick = false;

	SKMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SKMesh;

	bReplicates = true;
	SKMesh->SetIsReplicated(true);
}

void ASTDimensionDrift::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetGameState<ASTGameState>()->OnServerTravelReady.AddDynamic(this, &ASTDimensionDrift::HandleAllPlayerIsReady);
}

void ASTDimensionDrift::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASTDimensionDrift, SKMesh);
}

void ASTDimensionDrift::Interact_Implementation(UInteractComponent* InteractingComponent)
{
	ASTPlayerController* pc = Cast<ASTPlayerController>(InteractingComponent->GetOwner());

	USTPlayerUIComponent* uiComponent = pc->GetUIComponent();

	FGameplayTag loadoutTag = FSTGameplayTags::Get().Widget_Lobby_Loadout;
	FGameplayTag selectCharacterTag = FSTGameplayTags::Get().Widget_Lobby_CharacterSelect;
	FGameplayTag selectLevelTag = FSTGameplayTags::Get().Widget_Lobby_LevelSelect;

	uiComponent->AddPlayerID(pc->PlayerState->GetUniqueId());

	uiComponent->ServerRPCRegisterWidget(loadoutTag, Widget_LoadoutClass);
	uiComponent->ServerRPCRegisterWidget(selectCharacterTag, Widget_CharacterSelectClass);
	uiComponent->ServerRPCRegisterWidget(selectLevelTag, Widget_LevelSelectClass);

	if (pc->IsLocalController())
	{
		//uiComponent->RegisterWidget(loadoutTag, Widget_LoadoutClass);
		//uiComponent->RegisterWidget(selectCharacterTag, Widget_CharacterSelectClass);
		//uiComponent->RegisterWidget(selectLevelTag, Widget_LevelSelectClass);
		uiComponent->AddToViewport(loadoutTag);
	}
	else
	{
		//uiComponent->ClientRPCRegisterWidget(loadoutTag, Widget_LoadoutClass);
		//uiComponent->ClientRPCRegisterWidget(selectCharacterTag, Widget_CharacterSelectClass);
		//uiComponent->ClientRPCRegisterWidget(selectLevelTag, Widget_LevelSelectClass);
		uiComponent->ClientRPCAddToViewport(loadoutTag);
	}

	uiComponent->ServerRPCUpdateUI(loadoutTag);
	
	//if (pc->HasAuthority())
	//{
	//	uiComponent->AddPlayerID(pc->PlayerState->GetUniqueId());
	//	// 스스로 호출
	//	//uiComponent->ServerRPCUpdateUI(loadoutTag);
	//}
	//else
	//{
	//	uiComponent->ServerRPCRegisterPlayerID(pc->PlayerState->GetUniqueId());
	//	uiComponent->ServerRPCUpdateUI(loadoutTag);
	//}

	//if (pc->IsLocalController())
	//{
	//	uiComponent->AddPlayerID(pc->PlayerState->GetUniqueId());
	//	uiComponent->UpdateLoadoutUI();
	//	uiComponent->UpdatePlayerWeaponLayer();
	//}
	//else
	//{
	//	uiComponent->ServerRPCRegisterPlayerID(pc->PlayerState->GetUniqueId());
	//}
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

void ASTDimensionDrift::HandleAllPlayerIsReady(FGameplayTag NewGameLevel)
{
	GetWorld()->ServerTravel(GetLevelName(NewGameLevel));
}
