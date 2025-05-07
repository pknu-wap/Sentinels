// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interact/Tank/TankAnimInstance.h"
#include "Actors/Interact/Tank/InteractableTank.h"

UTankAnimInstance::UTankAnimInstance()
{
}

void UTankAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Tank = Cast<AInteractableTank>(TryGetPawnOwner());
}

void UTankAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Tank)
	{
		WheelSpeed = Tank->GetVelocity().Size2D();

		if (Tank->Controller)
		{
			FRotator Rotation = Tank->Controller->GetControlRotation();
			TurretAngle = Rotation.Yaw;
		}
	}
}
