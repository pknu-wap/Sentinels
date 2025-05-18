// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/STPlayerStatusComponent.h"
#include "Player/Inventory/ItemObject.h"
#include "Kismet/GameplayStatics.h"
#include "SubSystem/InventorySubsystem.h"
#include "Sentinels_LS.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bReplicateUsingRegisteredSubObjectList = true;
	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	EmptySlot = FInvSlotStruct();
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, Inventory);
}

const FInvSlotStruct& UInventoryComponent::GetItem(int InItemID) const
{
	for (auto& slot : Inventory)
	{
		if (slot.ItemID == InItemID)
		{
			return slot;
		}
	}

	return EmptySlot;
}

void UInventoryComponent::AddItem_Server_Implementation(int InItemID)
{
	USTPlayerStatusComponent* StatusComp = GetOwner()->GetComponentByClass<USTPlayerStatusComponent>();

	for (auto& slot : Inventory)
	{
		if (slot.ItemID == InItemID)
		{
			slot.Quantity++;

			if (slot.ItemObject)
				slot.ItemObject->AddQuantity(this, 1);

			if (StatusComp)
				StatusComp->CalculateStatus();

			return;
		}
	}

	// Get Item Info from Item Subsystem
	UGameInstance* GameInst = UGameplayStatics::GetGameInstance(this);
	if (!GameInst) return;
	UInventorySubsystem* InvSubSystem = GameInst->GetSubsystem<UInventorySubsystem>();
	if (!InvSubSystem) return;
	const FItemStruct* ItemInfo = InvSubSystem->GetItemInfo(InItemID);
	if (!ItemInfo) return;

	
	UItemObject* NewItem = NewObject<UItemObject>(this, ItemInfo->ItemClass);
	if (NewItem)
	{
		AddReplicatedSubObject(NewItem);
		Inventory.Add(FInvSlotStruct(InItemID, 1, NewItem));
		/*if(NewItem)
			NewItem->CalculateStatus(GetOwner());*/

		if (StatusComp)
			StatusComp->CalculateStatus();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent : Failed to Create"));
	}
}

void UInventoryComponent::OnRep_Inventory()
{
	ST_SUBLOG(LogTemp, Display, TEXT("Inventory Updated!!"));

	/*
		Update Player Status

	*/
	/*
		Update Item UI
	*/
}

void UInventoryComponent::LogInventory()
{
	UE_LOG(LogTemp, Display, TEXT("Controller %s : Log Inventory"), *GetOwner()->GetName());
	for (int i = 0; i < Inventory.Num(); i++)
	{
		UE_LOG(LogTemp, Display, TEXT("ID : %d"), Inventory[i].ItemID);
		if (Inventory[i].ItemObject)
			Inventory[i].ItemObject->LogItem();
	}
}