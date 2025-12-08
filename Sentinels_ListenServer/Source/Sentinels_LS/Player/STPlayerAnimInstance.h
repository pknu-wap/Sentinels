// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/STCharacterAnimInstanceBase.h"
#include "STPlayerAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FThrowLiftingActor);
DECLARE_MULTICAST_DELEGATE(FStartCheckNextInput);
DECLARE_MULTICAST_DELEGATE(FCheckNextAttack);
DECLARE_MULTICAST_DELEGATE(FSetWarpTarget_Q);
DECLARE_MULTICAST_DELEGATE(FSpawnSlash_Q);
DECLARE_MULTICAST_DELEGATE(FSetWarpTarget_Passive);
DECLARE_MULTICAST_DELEGATE(FSetWarpTarget_Step);
DECLARE_MULTICAST_DELEGATE(FApplyDamage)

UCLASS()
class SENTINELS_LS_API USTPlayerAnimInstance : public USTCharacterAnimInstanceBase
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void AnimNotify_StartCheckNextInput();

	UFUNCTION()
	void AnimNotify_CheckNextAttack();


public:
	FStartCheckNextInput Delegate_StartCheckNextInput;
	FCheckNextAttack Delegate_CheckNextAttack;


	/*
		GreatSword
	*/
protected:

public:

	/*
		Katana
	*/
protected:
	UFUNCTION()
	void AnimNotify_SetWarpTarget_Q();

	UFUNCTION()
	void AnimNotify_SpawnSlash_Q();

	UFUNCTION()
	void AnimNotify_SetWarpTarget_Passive();

	UFUNCTION()
	void AnimNotify_ApplyDamage();

public:
	FSetWarpTarget_Q Delegate_SetWarpTarget_Q;
	FSpawnSlash_Q Delegate_SpawnSlash_Q;
	FSetWarpTarget_Passive Delegate_SetWarpTarget_Passive;
	FApplyDamage Delegate_ApplyDamage;

	/*
		DualBlade
	*/
protected:

public:

	/*
		Normal
	*/
protected:
	UFUNCTION()
	void AnimNotify_ThrowLiftingActor();

	UFUNCTION()
	void AnimNotify_SetWarpTarget_Step();

public:
	FThrowLiftingActor Delegate_ThrowLiftingActor;
	FSetWarpTarget_Step Delegate_SetWarpTarget_Step;
};
