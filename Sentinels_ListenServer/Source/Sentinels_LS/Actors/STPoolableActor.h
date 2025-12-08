// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STPoolableActor.generated.h"

UCLASS()
class SENTINELS_LS_API ASTPoolableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTPoolableActor();

public:	
	UFUNCTION(BlueprintCallable)
	virtual void Activate(const FVector ActivateLocation = FVector::ZeroVector, const FRotator ActivateRotation = FRotator::ZeroRotator);

	UFUNCTION(BlueprintCallable)
	virtual void Deactivate();

	UFUNCTION(BlueprintImplementableEvent)
	void OnActorActivated();

	UFUNCTION(BlueprintImplementableEvent)
	void OnActorDeactivated();

public:
	FORCEINLINE bool IsActivated() const { return bIsActivated; }

private:
	bool bIsActivated = false;
};
