// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MissionObject/Interactable/InteractableMissionObject.h"
#include "Rift.generated.h"

class UInteractComponent;
class ASTEnemyBase;
class USpawnEnemyComponent;

UCLASS()
class SENTINELS_LS_API ARift : public AInteractableMissionObject
{
	GENERATED_BODY()

	ARift();

/*
	Actor Interface
*/
protected:
	virtual void BeginPlay() override;

/*
	Interactive Interface
*/
protected:
	virtual void Interact_Implementation(UInteractComponent* InteractingComponent) override;
	virtual void Interact_Finish_Implementation(UInteractComponent* InteractingComponent) override;

/*
	Initial Spawn Enemy when Mission Activated
*/
public:
	void SpawnInitialEnemy();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USpawnEnemyComponent> SpawnComponent;

/*
	Repair Success
*/
protected:
	void RepairSuccessed();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InteractionHoldTime = 7.5f;

private:
	UPROPERTY()
	UInteractComponent* InteractedComponent;

	FTimerHandle Handle_Hold;
};
