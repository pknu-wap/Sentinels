// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TankAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API UTankAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UTankAnimInstance();
	
public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	void SetController(AController* inController) { Controller = inController; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AInteractableTank* Tank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AController* Controller;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WheelSpeedOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WheelSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TurretAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GunElevation;
};
