// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/GRIM/STGRIMAnimInstance.h"

void USTGRIMAnimInstance::AnimNotify_UltimateFire_Red()
{
	Delegate_UltimateFire_Red.Broadcast();
}
