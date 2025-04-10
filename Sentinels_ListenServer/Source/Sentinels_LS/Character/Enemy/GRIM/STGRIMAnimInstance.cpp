// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/GRIM/STGRIMAnimInstance.h"

void USTGRIMAnimInstance::AnimNotify_GrimPrimaryFire()
{
	Delegate_GrimPrimaryFire.Broadcast();
}
