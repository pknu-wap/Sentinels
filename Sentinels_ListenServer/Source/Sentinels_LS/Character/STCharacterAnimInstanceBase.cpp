// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/STCharacterAnimInstanceBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

USTCharacterAnimInstanceBase::USTCharacterAnimInstanceBase()
{
}

void USTCharacterAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(TryGetPawnOwner());
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
