// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESTClassType : uint8
{
	GreatSword,
	Katana,
	DualBlade,
	Magician	
};

UENUM(BlueprintType)
enum class ESTStatusType : uint8
{
	None,
	HP,
	ATK,
	DEF,
	MovementSpeed,
	AttackSpeed,
	DamageIncreaseRate,
	CriticalDamagePercent,
	CriticalRate,
	CDR,
};

UENUM(BlueprintType)
enum class ESTBuffType : uint8
{
	None,
	HP,
	ATK,
	DEF,
	MovementSpeed,
	AttackSpeed,
	DamageIncreaseRate,
	CriticalDamagePercent,
	CriticalRate,
	CDR,
};