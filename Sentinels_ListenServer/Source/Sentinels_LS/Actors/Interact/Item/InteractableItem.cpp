// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interact/Item/InteractableItem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InventoryComponent.h"

void AInteractableItem::BeginPlay()
{
	Super::BeginPlay();
	if (Item_DataRow.DataTable)
	{
		FItemStruct* itemData =
			Item_DataRow.DataTable->FindRow<FItemStruct>(Item_DataRow.RowName, Item_DataRow.RowName.ToString());

		if (!itemData)
		{
			UE_LOG(LogTemp, Warning, TEXT("AInteractableItem : %s Failed to initialize Item ID!!"), *GetName());
			return;
		}

		ItemID = itemData->ItemID;
	}
}

void AInteractableItem::Interact()
{
	if (Item_DataRow.DataTable)
	{
		APawn* pawn = UGameplayStatics::GetPlayerPawn(this, 0);
		if (!pawn) return;

		UInventoryComponent* InvComp = pawn->GetComponentByClass<UInventoryComponent>();
		if (InvComp)
		{
			InvComp->AddItem_Server(ItemID);
			UE_LOG(LogTemp, Display, TEXT(" AInteractableItem::Interact!"));
		}
	}
}