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

ASTDimensionDrift::ASTDimensionDrift()
{
	PrimaryActorTick.bCanEverTick = false;

	SKMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SKMesh;
}

void ASTDimensionDrift::BeginPlay()
{
	Super::BeginPlay();
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
	FGameplayTag selectTag = FSTGameplayTags::Get().Widget_Lobby_CharacterSelect;

	uiComponent->ClientRPCRegisterWidget(loadoutTag, Widget_LoadoutClass);
	uiComponent->ClientRPCRegisterWidget(selectTag, Widget_CharacterSelectClass);

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
