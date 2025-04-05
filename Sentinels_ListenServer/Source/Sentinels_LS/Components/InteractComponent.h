// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegates/Delegate.h"
#include "InteractComponent.generated.h"

class IInteractiveInterface;

// DECLARE_DELEGATE(FOn);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartInteractHold, float, HoldingTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishInteractHold);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENTINELS_LS_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

/*
	Interact
*/
public:
	UFUNCTION(Server, Reliable)
	void Interact_Server();

	UFUNCTION(Server, Reliable)
	void Interact_Finish_Server();


/*
	Interact Hold (Hostage, Rift, . . .)
*/
public:
	UFUNCTION(Client, Reliable)
	void StartInteractHold_Client(float InHoldingTime);

	UFUNCTION(Client, Reliable)
	void FinishInteractHold_Client();

public:
	UPROPERTY(BlueprintAssignable)
	FOnStartInteractHold Delegate_StartInteractHold;

	UPROPERTY(BlueprintAssignable)
	FOnFinishInteractHold Delegate_FinishInteractHold;

private:
	bool bIsHolding = false;


/*
	Tracing For Interact
*/
protected:
	void FindInteractiveActor();

protected:
	UPROPERTY(EditAnywhere)
	float Interact_Range = 500;

	UPROPERTY(EditAnywhere)
	float Interact_Radius = 50;

private:
	IInteractiveInterface* InteractiveObject;
};
