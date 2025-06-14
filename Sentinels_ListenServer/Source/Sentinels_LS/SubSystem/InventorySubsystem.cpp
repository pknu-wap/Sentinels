// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/InventorySubsystem.h"
#include "Kismet/KismetMathLibrary.h"

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
	for (const FName& RowName : rowNames)
	{
		FItemStruct* ItemInfo = ItemDB->FindRow<FItemStruct>(RowName, TEXT("FindRow"));
		if (ItemInfo != nullptr)
		{
			ItemMap.Add(ItemInfo->ItemID, *ItemInfo);
		}
	}

	CombatItemDB = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/Sentinels/Inventory/DataTables/DB_Items_Combat.DB_Items_Combat'"));
	if (CombatItemDB == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySubsystem : Failed to Load ItemDB !!! "));
	}

	CombatStartID = 10000;
	rowNames = CombatItemDB->GetRowNames();
	for (const FName& RowName : rowNames)
	{
		FItemStruct* ItemInfo = CombatItemDB->FindRow<FItemStruct>(RowName, TEXT("FindRow"));
		if (ItemInfo != nullptr)
		{
			CombatStartID = FMath::Min(CombatStartID, ItemInfo->ItemID);
			CombatItemMap.Add(ItemInfo->ItemID, *ItemInfo);
		}
	}

	UniqueItemDB = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/Sentinels/Inventory/DataTables/DB_Items_Unique.DB_Items_Unique'"));
	if (UniqueItemDB == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySubsystem : Failed to Load ItemDB !!! "));
	}

	UniqueStartID = 10000;
	rowNames = UniqueItemDB->GetRowNames();
	for (const FName& RowName : rowNames)
	{
		FItemStruct* ItemInfo = UniqueItemDB->FindRow<FItemStruct>(RowName, TEXT("FindRow"));
		if (ItemInfo != nullptr)
		{
			UniqueStartID = FMath::Min(UniqueStartID, ItemInfo->ItemID);
			UniqueItemMap.Add(ItemInfo->ItemID, *ItemInfo);
		}
	}
}

const FItemStruct* UInventorySubsystem::GetItemInfo(int itemID) const
{
	return ItemMap.Find(itemID);
}

const FItemStruct* UInventorySubsystem::GetRandomCombatItemInfo() const
{
	int itemID = UKismetMathLibrary::RandomIntegerInRange(0, CombatItemMap.Num() - 1);

	return CombatItemMap.Find(CombatStartID + itemID);
}

const FItemStruct* UInventorySubsystem::GetRandomUniqueItemInfo() const
{
	int itemID = UKismetMathLibrary::RandomIntegerInRange(0, UniqueItemMap.Num() - 1);

	return UniqueItemMap.Find(UniqueStartID + itemID);
}

int UInventorySubsystem::GetRandomCombatItemID() const
{
	if (CombatItemMap.Num() == 0)
		return -1;

	int itemID = UKismetMathLibrary::RandomIntegerInRange(0, CombatItemMap.Num() - 1);

	return CombatStartID + itemID;
}

int UInventorySubsystem::GetRandomUniqueItemID() const
{
	if (UniqueItemMap.Num() == 0)
		return -1;

	int itemID = UKismetMathLibrary::RandomIntegerInRange(0, UniqueItemMap.Num() - 1);

	return UniqueStartID + itemID;
}
