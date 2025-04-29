// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "STCharacterAnimInstanceBase.generated.h"

class ASTCharacterBase;

DECLARE_MULTICAST_DELEGATE(FPrimaryFire);
DECLARE_MULTICAST_DELEGATE(FUltimateFire);

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

protected:
	UFUNCTION()
	void AnimNotify_PrimaryFire();

	UFUNCTION()
	void AnimNotify_UltimateFire();

public:
	FPrimaryFire Delegate_PrimaryFire;
	FUltimateFire Delegate_UltimateFire;
};
