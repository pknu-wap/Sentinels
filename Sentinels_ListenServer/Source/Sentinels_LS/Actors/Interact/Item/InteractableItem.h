// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interact/InteractableActor.h"
#include "InteractableItem.generated.h"

class UInteractComponent;

UCLASS()
class SENTINELS_LS_API AInteractableItem : public AInteractableActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Interact_Implementation(UInteractComponent* InteractingComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetItemID();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	bool bUseItemHandle = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FDataTableRowHandle ItemHandle_DataRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int ItemID;
};
