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

		UpdatePitchOffset();
		UpdateYawOffset();
		UpdateTurnInPlace();
	}
}

void USTCharacterAnimInstanceBase::UpdatePitchOffset()
{
	if (Character)
	{
		PitchOffset = UKismetMathLibrary::NormalizedDeltaRotator(Character->GetBaseAimRotation(), Character->GetActorRotation()).Pitch;
	}
}

void USTCharacterAnimInstanceBase::UpdateYawOffset()
{
	if (Character)
	{
		YawLastTick = Yaw;
		Yaw = Character->GetActorRotation().Yaw;

		if (GroundSpeed > 0.f)
		{
			RootYawOffset = 0.f;
			return;
		}

		float YawChangeOverFrame = YawLastTick - Yaw;
		RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset + YawChangeOverFrame);
	}
}

void USTCharacterAnimInstanceBase::UpdateTurnInPlace()
{
	if (GetCurveValue(FName("TurnYawWeight")))
	{
		float direction = 0;
		(RootYawOffset > 0.f) ? direction = -1.f : direction = 1.f;

		DistanceCurveValueLastTick = DistanceCurveValue;
		DistanceCurveValue = GetCurveValue(DistanceCurveName);

		RootYawOffset = RootYawOffset - (DistanceCurveValueLastTick - DistanceCurveValue) * direction;

		float absRootYawOffset = UKismetMathLibrary::Abs(RootYawOffset);
		if (absRootYawOffset > MaxTurnAngle)
		{
			float yawToSubtract = absRootYawOffset - MaxTurnAngle;
			float yatMultiplier = RootYawOffset > 0.f ? 1.f : -1.f;
			RootYawOffset = RootYawOffset - yawToSubtract * yatMultiplier;
		}
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

void USTCharacterAnimInstanceBase::AnimNotify_DissolveStart()
{
	Delegate_DissolveStart.Broadcast();
}
