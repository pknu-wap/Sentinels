// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STStructs.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateInventory);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENTINELS_LS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintCallable)
	float AdjustFinalDamage(float DamageAmount, FDamageEvent const& DamageEvent, AActor* DamagedActor);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FInvSlotStruct> GetInventoryCopy() const { return Inventory; }

	FORCEINLINE const TArray<FInvSlotStruct>& GetInventory() const { return Inventory; }
	const FInvSlotStruct& GetItem(int InItemID) const;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void AddItem_Server(int InItemID);

protected:
	UFUNCTION()
	void OnRep_Inventory();

	UPROPERTY(ReplicatedUsing = OnRep_Inventory)
	TArray<FInvSlotStruct> Inventory;

	FInvSlotStruct EmptySlot;


/*
	Debug
*/
public:
	UFUNCTION(BlueprintCallable)
	void LogInventory();

	UPROPERTY(BlueprintAssignable)
	FOnUpdateInventory OnUpdatreInventory;
};
