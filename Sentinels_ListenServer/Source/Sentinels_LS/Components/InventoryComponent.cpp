// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, Inventory);
}

void UInventoryComponent::AddItem_Server_Implementation(int InItemID)
{
	for (auto& slot : Inventory)
	{
		if (slot.ItemID == InItemID)
		{
			slot.Quantity++;
			return;
		}
	}

	Inventory.Add(FInvSlotStruct(InItemID, 1));
}

void UInventoryComponent::LogInventory()
{
	UE_LOG(LogTemp, Display, TEXT("Controller %s : Log Inventory"), *GetOwner()->GetName());
	for (int i = 0; i < Inventory.Num(); i++)
	{
		UE_LOG(LogTemp, Display, TEXT("ID : %d"), Inventory[i].ItemID);
	}
}

void UInventoryComponent::OnRep_Inventory()
{
	UE_LOG(LogTemp, Display, TEXT("UInventoryComponent : Inventory Updated!!"));

	/*
		Update Player Status
	*/

	/*
		Update Item UI
	*/
}