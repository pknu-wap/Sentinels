// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/STCharacterBase.h"
#include "STPoolableCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API ASTPoolableCharacter : public ASTCharacterBase
{
	GENERATED_BODY()

public:
	ASTPoolableCharacter(const FObjectInitializer& object_initializer);
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void Activate(const FVector ActivateLocation = FVector::ZeroVector, const FRotator ActivateRotation = FRotator::ZeroRotator);
	
	UFUNCTION(BlueprintCallable)
	virtual void Deactivate();

public:
	FORCEINLINE bool IsActivated() const { return bIsActivated; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActivated = true;
};
