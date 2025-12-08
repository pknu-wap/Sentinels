// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "STCharacterAnimInstanceBase.generated.h"

class ASTCharacterBase;

DECLARE_MULTICAST_DELEGATE(FPrimaryFire);
DECLARE_MULTICAST_DELEGATE(FUltimateFire);
DECLARE_MULTICAST_DELEGATE(FDissolveStart);

UCLASS()
class SENTINELS_LS_API USTCharacterAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	USTCharacterAnimInstanceBase();

public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Movement")
	float GroundSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Movement")
	float Angle = 0.f;

	UPROPERTY(BlueprintReadOnly, category = "Movement")
	class UCharacterMovementComponent* MovementComp;

protected:
	UPROPERTY(BlueprintReadOnly, category = "Character")
	class ASTCharacterBase* Character;


/*
	Turn In Place
*/
protected:
	void UpdatePitchOffset();
	void UpdateYawOffset();
	void UpdateTurnInPlace();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Movement")
	float PitchOffset = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Movement")
	FName YawCurveName = FName("TurnYawWeight");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Movement")
	float RootYawOffset = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Movement")
	float ActualYaw = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Movement")
	float MaxTurnAngle = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Movement")
	float Yaw = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Movement")
	float YawLastTick = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Movement")
	FName DistanceCurveName = FName("DistanceCurve");

	UPROPERTY()
	float DistanceCurveValue = 0.f;

	UPROPERTY()
	float DistanceCurveValueLastTick = 0.f;


/*
	AnimNotify
*/
protected:
	UFUNCTION()
	void AnimNotify_PrimaryFire();

	UFUNCTION()
	void AnimNotify_UltimateFire();

	UFUNCTION()
	void AnimNotify_DissolveStart();

public:
	FPrimaryFire Delegate_PrimaryFire;
	FUltimateFire Delegate_UltimateFire;
	FDissolveStart Delegate_DissolveStart;
};
