// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "STStructs.h"
#include "InventorySubsystem.generated.h"

/**
 * 
 */
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

	UFUNCTION(BlueprintCallable)
	int GetRandomCombatItemID() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TMap<int, FItemStruct> ItemMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TMap<int, FItemStruct> CombatItemMap;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UDataTable> ItemDB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UDataTable> CombatItemDB;
};
