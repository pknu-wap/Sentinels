// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interact/InteractableCharacter.h"
#include "Actors/MissionObject/MissionObject.h"
#include "InteractableHostage.generated.h"

class USpawnEnemyComponent;

UCLASS()
class SENTINELS_LS_API AInteractableHostage : public AInteractableCharacter, public IMissionInterface
{
	GENERATED_BODY()

public:
	AInteractableHostage();

	/*
		Actor Interface
	*/
protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;


	/*
		Hostage
	*/
public:
	void SetHostageID(int InHostageID) { HostageID = InHostageID; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int HostageID = 0;


	/*
		Interactive Interaface
	*/
public:
	virtual void Interact_Implementation(UInteractComponent* InteractingComponent) override;
	virtual void Interact_Finish_Implementation(UInteractComponent* InteractingComponent) override;

	virtual bool IsSuccessed_Implementation() override { return bIsSuccessed; }


	/*
		Spawn Enemy
	*/
public:
	void StopSpawnEnemy();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USpawnEnemyComponent> SpawnComponent;


	/*
		Rescue Success
	*/
protected:
	void RescueSuccessed();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InteractionHoldTime = 7.5f;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsSuccessed = false;

public:
	FMissionConditionUpdate Delegate_MissionConditionUpdate;

private:
	UPROPERTY()
	UInteractComponent* InteractedComponent;

	FTimerHandle Handle_Hold;
};
