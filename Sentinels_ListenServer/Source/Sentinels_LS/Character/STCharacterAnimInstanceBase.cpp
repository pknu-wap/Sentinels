// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/STCharacterAnimInstanceBase.h"
#include "Character/STCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

USTCharacterAnimInstanceBase::USTCharacterAnimInstanceBase()
{
}

void USTCharacterAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ASTCharacterBase>(TryGetPawnOwner());
	if (IsValid(Character))
		MovementComp = Character->GetCharacterMovement();
}

void USTCharacterAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (MovementComp && Character)
	{
		GroundSpeed = Character->GetVelocity().Size2D();
		FVector vRotation = UKismetMathLibrary::InverseTransformDirection(Character->GetActorTransform(), Character->GetVelocity());
		FRotator rotation = vRotation.Rotation();
		Angle = rotation.Yaw;
	}
}

void USTCharacterAnimInstanceBase::AnimNotify_PrimaryFire()
{
	Delegate_PrimaryFire.Broadcast();
}

void USTCharacterAnimInstanceBase::AnimNotify_UltimateFire()
{
	Delegate_UltimateFire.Broadcast();
}
