// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "STEnums.h"
#include "STPlayerState.generated.h"

class UInventoryComponent;
class USTPlayerStatusComponent;

UCLASS()
class SENTINELS_LS_API ASTPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ASTPlayerState();

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

protected:
	virtual void SeamlessTravelTo(class APlayerState* NewPlayerState) override;
	virtual void CopyProperties(APlayerState* PlayerState) override;

public:
	void ClearAllState();
	void UpdatePlayerClass(ESTClassType InClassType);

public:
	UPROPERTY()
	bool bIsInitialized = false;

protected:
	UPROPERTY()
	ESTClassType ClassType;

	/*
		Inventory
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	/*
		Status
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	TObjectPtr<USTPlayerStatusComponent> StatusComponent;

};
