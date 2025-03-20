// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/InventorySubsystem.h"

UInventorySubsystem::UInventorySubsystem()
{
}

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	ItemDB = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/Sentinels/Inventory/DataTables/DB_Items.DB_Items'"));
	if (ItemDB == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySubsystem : Failed to Load ItemDB !!! "));
	}

	TArray<FName> rowNames = ItemDB->GetRowNames();
	for (FName RowName : rowNames)
	{
		FItemStruct* ItemInfo = ItemDB->FindRow<FItemStruct>(RowName, TEXT("FindRow"));
		if (ItemInfo != nullptr)
		{
			ItemMap.Add(ItemInfo->ItemID, *ItemInfo);
		}
	}
}

const FItemStruct* UInventorySubsystem::GetItemInfo(int itemID) const
{
	return ItemMap.Find(itemID);
}
