// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interact/InteractableActor.h"
#include "InteractableItem.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API AInteractableItem : public AInteractableActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Interact() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FDataTableRowHandle Item_DataRow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int ItemID;
};
