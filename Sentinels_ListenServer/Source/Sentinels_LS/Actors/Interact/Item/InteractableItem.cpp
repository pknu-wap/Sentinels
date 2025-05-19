// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interact/Item/InteractableItem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InventoryComponent.h"
#include "Components/InteractComponent.h"
#include "SubSystem/InventorySubsystem.h"

void AInteractableItem::BeginPlay()
{
	Super::BeginPlay();

	if (bUseItemHandle && ItemHandle_DataRow.DataTable)
	{
		FItemStruct* itemData =
			ItemHandle_DataRow.DataTable->FindRow<FItemStruct>(ItemHandle_DataRow.RowName, ItemHandle_DataRow.RowName.ToString());

		if (!itemData)
		{
			UE_LOG(LogTemp, Warning, TEXT("AInteractableItem : %s Failed to initialize Item ID!!"), *GetName());
			return;
		}

		ItemID = itemData->ItemID;
	}
	else
	{
		SetItemID();
	}
}

void AInteractableItem::Interact_Implementation(UInteractComponent* InteractingComponent)
{
	APlayerController* PC = Cast<APlayerController>(InteractingComponent->GetOwner());
	if (!PC) return;

	APawn* pawn = PC->GetPawn();
	if (!pawn) return;

	UInventoryComponent* InvComp = pawn->GetComponentByClass<UInventoryComponent>();
	if (InvComp)
	{
		// bIsInteractable = false;
		InvComp->AddItem_Server(ItemID);
		UE_LOG(LogTemp, Display, TEXT(" AInteractableItem::Interact - Item ID : %d"), ItemID);
	}
}