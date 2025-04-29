// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interact/STDimensionDrift.h"
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

	GetWorld()->GetGameState<ASTGameState>()->OnAllPlayerIsReady.AddDynamic(this, &ASTDimensionDrift::HandleAllPlayerIsReady);
}

void ASTDimensionDrift::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void ASTDimensionDrift::Interact(UInteractComponent* InteractingComponent)
{
	ASTPlayerController* pc = Cast<ASTPlayerController>(InteractingComponent->GetOwner());

	USTPlayerUIComponent* uiComponent = pc->GetUIComponent();

	FGameplayTag loadoutTag = FSTGameplayTags::Get().Widget_Lobby_Loadout;
	FGameplayTag selectCharacterTag = FSTGameplayTags::Get().Widget_Lobby_CharacterSelect;
	FGameplayTag selectLevelTag = FSTGameplayTags::Get().Widget_Lobby_LevelSelect;

	uiComponent->ClientRPCRegisterWidget(loadoutTag, Widget_LoadoutClass);
	uiComponent->ClientRPCRegisterWidget(selectCharacterTag, Widget_CharacterSelectClass);
	uiComponent->ClientRPCRegisterWidget(selectLevelTag, Widget_LevelSelectClass);

	uiComponent->ClientRPCAddToViewport(loadoutTag);

	if (pc->IsLocalController())
	{
		uiComponent->AddPlayerID(pc->PlayerState->GetUniqueId());
		uiComponent->UpdateLoadoutUI();
	}
	else
	{
		uiComponent->ServerRPCRegisterPlayerID(pc->PlayerState->GetUniqueId());
	}
}

void ASTDimensionDrift::Interact_Finish(UInteractComponent* InteractingComponent)
{
}

void ASTDimensionDrift::ShowInteractiveUI(UInteractComponent* InteractingComponent)
{
	
}

void ASTDimensionDrift::HideInteractiveUI(UInteractComponent* InteractingComponent)
{
}

void ASTDimensionDrift::HandleAllPlayerIsReady(FGameplayTag NewGameLevel)
{
	GetWorld()->ServerTravel(GetLevelName(NewGameLevel));
}
