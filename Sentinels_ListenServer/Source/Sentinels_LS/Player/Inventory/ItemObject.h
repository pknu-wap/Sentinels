// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/NetworkObject.h"
#include "ItemObject.generated.h"

class USTPlayerStatusComponent;
class UInventoryComponent;

UCLASS()
class SENTINELS_LS_API UItemObject : public UNetworkObject
{
	GENERATED_BODY()

/*
	UObject Interface
*/
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
/*
	Item Base Attribute
*/
public:
	FORCEINLINE int GetItemID() const { return ItemID; }

	UFUNCTION(BlueprintImplementableEvent)
	void LogItem();

	UFUNCTION(BlueprintImplementableEvent)
	void Initialize(class AActor* OwnerActor);

	void AddQuantity(UInventoryComponent* Inventory, int InQuantity);

	UFUNCTION()
	void OnRep_Quantity();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int ItemID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemDescription;

	UPROPERTY(ReplicatedUsing = OnRep_Quantity, BlueprintReadOnly)
	int Quantity = 1;

/*
	Normal Status
*/
public:
	UFUNCTION(BlueprintImplementableEvent)
	void CalculateStatus(class AActor* OwnerActor);


/*
	Event
*/
public:
	UFUNCTION(BlueprintImplementableEvent)
	float AdjustFinalDamage(float DamageAmount, FDamageEvent const& DamageEvent, AActor* DamagedActor);

	UFUNCTION(BlueprintImplementableEvent)
	void OnAttackSuccess(float DamageAmount, FDamageEvent const& DamageEvent, AActor* DamagedActor);
};
