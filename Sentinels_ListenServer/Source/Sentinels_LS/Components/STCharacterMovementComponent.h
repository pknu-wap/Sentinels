// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "STCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API USTCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

protected:
	FVector ConstrainAnimRootMotionVelocity(const FVector& RootMotionVelocity, const FVector& CurrentVelocity) const override;

	
};
