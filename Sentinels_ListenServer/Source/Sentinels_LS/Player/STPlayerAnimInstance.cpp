// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/STPlayerAnimInstance.h"

void USTPlayerAnimInstance::AnimNotify_StartCheckNextInput()
{
	Delegate_StartCheckNextInput.Broadcast();
}

void USTPlayerAnimInstance::AnimNotify_CheckNextAttack()
{
	Delegate_CheckNextAttack.Broadcast();
}

void USTPlayerAnimInstance::AnimNotify_SetWarpTarget_Q()
{
	Delegate_SetWarpTarget_Q.Broadcast();
}

void USTPlayerAnimInstance::AnimNotify_SpawnSlash_Q()
{
	Delegate_SpawnSlash_Q.Broadcast();
}

void USTPlayerAnimInstance::AnimNotify_ThrowLiftingActor()
{
	Delegate_ThrowLiftingActor.Broadcast();
}
