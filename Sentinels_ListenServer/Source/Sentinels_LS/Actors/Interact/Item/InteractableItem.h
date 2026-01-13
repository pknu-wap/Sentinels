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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:
	virtual void Interact_Implementation(UInteractComponent* InteractingComponent) override;
	virtual void ShowInteractiveUI_Implementation(UInteractComponent* InteractingComponent) override;
	virtual void HideInteractiveUI_Implementation(UInteractComponent* InteractingComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetItemID();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	bool bUseItemHandle = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FDataTableRowHandle ItemHandle_DataRow;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Item")
	int ItemID;
};
