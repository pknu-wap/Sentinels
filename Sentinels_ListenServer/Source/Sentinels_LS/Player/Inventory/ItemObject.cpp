// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Inventory/ItemObject.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InventoryComponent.h"

void UItemObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemObject, Quantity);
}

void UItemObject::AddQuantity(UInventoryComponent* Inventory, int InQuantity)
{
	if (Inventory && Inventory->GetOwnerRole() == ROLE_Authority)
	{
		Quantity += InQuantity;
		OnRep_Quantity();
	}
}

void UItemObject::OnRep_Quantity()
{
	/*
		Update UI
	*/
}

void UItemObject::OnItemInitaillyAcquired_Implementation()
{
}

float UItemObject::AdjustFinalDamage_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AActor* DamagedActor)
{
	return DamageAmount;
}

void UItemObject::OnAttackSuccess_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AActor* DamagedActor)
{
}

float UItemObject::OnDamaged_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return DamageAmount;
}
