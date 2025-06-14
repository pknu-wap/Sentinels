// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "STStructs.h"
#include "InventorySubsystem.generated.h"

class AInteractableItem;

UCLASS()
class SENTINELS_LS_API UInventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UInventorySubsystem();

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	const FItemStruct* GetItemInfo(int itemID) const;
	const FItemStruct* GetRandomCombatItemInfo() const;
	const FItemStruct* GetRandomUniqueItemInfo() const;

	UFUNCTION(BlueprintCallable)
	int GetRandomCombatItemID() const;

	UFUNCTION(BlueprintCallable)
	int GetRandomUniqueItemID() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TMap<int, FItemStruct> ItemMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TMap<int, FItemStruct> CombatItemMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TMap<int, FItemStruct> UniqueItemMap;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UDataTable> ItemDB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UDataTable> CombatItemDB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UDataTable> UniqueItemDB; 

private:
	UPROPERTY()
	int CombatStartID;

	UPROPERTY()
	int UniqueStartID;
};
