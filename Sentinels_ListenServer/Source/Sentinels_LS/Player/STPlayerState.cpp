// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/STPlayerState.h"
#include "Components/InventoryComponent.h"
#include "Components/STPlayerStatusComponent.h"
#include "Net/UnrealNetwork.h"

ASTPlayerState::ASTPlayerState()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComp"));
	InventoryComponent->SetIsReplicated(true);

	StatusComponent = CreateDefaultSubobject<USTPlayerStatusComponent>(TEXT("StatusComp"));
	StatusComponent->SetIsReplicated(true);
}

void ASTPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASTPlayerState, InventoryComponent);
	DOREPLIFETIME(ASTPlayerState, StatusComponent);
}

void ASTPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void ASTPlayerState::SeamlessTravelTo(APlayerState* NewPlayerState)
{
	Super::SeamlessTravelTo(NewPlayerState);
}

void ASTPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	if (ASTPlayerState* PS = Cast<ASTPlayerState>(PlayerState))
	{
		PS->ClassType = ClassType;
		PS->bIsInitialized = bIsInitialized;

		if (PS->bIsInitialized)
		{
			if (UInventoryComponent* newInventory = PlayerState->GetComponentByClass<UInventoryComponent>())
			{
				newInventory->SetInventory(InventoryComponent->GetInventory());
			}

			if (USTPlayerStatusComponent* newStatusComponent = PlayerState->GetComponentByClass<USTPlayerStatusComponent>())
			{
				newStatusComponent->CopyProperties(StatusComponent);
			}
		}
	}
}

void ASTPlayerState::UpdatePlayerClass(ESTClassType InClassType)
{
	// Should Check Player is in InGame!
	if (ClassType != InClassType || !bIsInitialized)
	{
		ClassType = InClassType;
		bIsInitialized = true;

		StatusComponent->InitializeStatus(InClassType);
	}
}
