// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STCharacterMovementComponent.h"

FVector USTCharacterMovementComponent::ConstrainAnimRootMotionVelocity(const FVector& RootMotionVelocity, const FVector& CurrentVelocity) const
{
	return Super::ConstrainAnimRootMotionVelocity(RootMotionVelocity, CurrentVelocity);
}
