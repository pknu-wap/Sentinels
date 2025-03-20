// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STPlayerStatusComponent.h"
#include "Components/InventoryComponent.h"
#include "SubSystem/InventorySubsystem.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USTPlayerStatusComponent::USTPlayerStatusComponent()
	: BaseMaxHP(100.f), MaxCDR(0.5f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USTPlayerStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	HP = MaxHP = BaseMaxHP;
	CDR = MaxCDR;
}

void USTPlayerStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USTPlayerStatusComponent, MaxHP);
	DOREPLIFETIME(USTPlayerStatusComponent, HP);

	DOREPLIFETIME(USTPlayerStatusComponent, ATK);
	DOREPLIFETIME(USTPlayerStatusComponent, DEF);

	DOREPLIFETIME(USTPlayerStatusComponent, MaxDamageIncreaseRate);
	DOREPLIFETIME(USTPlayerStatusComponent, DamageIncreaseRate);

	DOREPLIFETIME(USTPlayerStatusComponent, MaxCDR);
	DOREPLIFETIME(USTPlayerStatusComponent, CDR);

	DOREPLIFETIME(USTPlayerStatusComponent, MaxCriticalRate);
	DOREPLIFETIME(USTPlayerStatusComponent, CriticalRate);
}

void USTPlayerStatusComponent::CalculateStatus()
{
	UInventorySubsystem* InvSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	UInventoryComponent* InvComp = GetOwner()->GetComponentByClass<UInventoryComponent>();
	if (!InvComp || !InvSubSystem) return;

	// Reset Status 
	MaxHP = BaseMaxHP; DEF = 0; DamageIncreaseRate = 0; CDR = 0; CriticalRate = 0;

	// ReCalculate Status
	const TArray<FInvSlotStruct>& Inventory =  InvComp->GetInventory();
	for (auto& Slot : Inventory)
	{
		const FItemStruct* itemInfo = InvSubSystem->GetItemInfo(Slot.ItemID);
		if (!itemInfo) continue;

		if(itemInfo->MaxHP > 0)
			MaxHP += itemInfo->MaxHP * Slot.Quantity;

		if(itemInfo->DEF > 0)
			DEF += itemInfo->DEF * Slot.Quantity;

		if(itemInfo->DamageIncreaseRate > 0)
			DamageIncreaseRate = FMath::Clamp(DamageIncreaseRate + itemInfo->DamageIncreaseRate * Slot.Quantity, 0, MaxDamageIncreaseRate);

		if(itemInfo->CDR > 0)
			CDR = FMath::Clamp(CDR + itemInfo->CDR * Slot.Quantity, 0, MaxCDR);

		if(itemInfo->CriticalRate > 0)
			CriticalRate = FMath::Clamp(CriticalRate + itemInfo->CriticalRate * Slot.Quantity, 0, MaxCriticalRate);
	}
}

void USTPlayerStatusComponent::OnRep_HPUpdated()
{
	UE_LOG(LogTemp, Display, TEXT("USTPlayerStatusComponent::OnRep_HPUpdated Called!"));
	// Update UI

	// Check Died
}

