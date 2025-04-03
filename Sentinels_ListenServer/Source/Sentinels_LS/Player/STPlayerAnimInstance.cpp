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
