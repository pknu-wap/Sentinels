// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESTClassType : uint8
{
	None,
	GreatSword,
	Katana,
	DualBlade,
	Magician	
};

UENUM(BlueprintType)
enum class ESKParts : uint8
{
	Head UMETA(DisplayName = "Head"),
	Hood UMETA(DisplayName = "Hood"),
	LongHair UMETA(DisplayName = "LongHair"),
	Glasses UMETA(DisplayName = "Glasses"),
	UpperBody UMETA(DisplayName = "UpperBody"),
	Backpack UMETA(DisplayName = "Backpack"),
	Hand_L UMETA(DisplayName = "Hand_L"),
	Hand_R UMETA(DisplayName = "Hand_R"),
	BottomBody UMETA(DisplayName = "BottomBody"),
	Feet UMETA(DisplayName = "Feet"),
	GreatSword UMETA(DisplayName = "GreatSword"),
	Katana UMETA(DisplayName = "Katana"),
	Blade_L UMETA(DisplayName = "Blade_L"),
	Blade_R UMETA(DisplayName = "Blade_R"),
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ESTStatusType : uint8
{
	None,
	HP,
	HPRegen,
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
	HPRegen,
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
enum class EMissionProgressState : uint8
{
	None,
	Registered,
	Activated,
	Cleared,
	Failed
};

UENUM(BlueprintType)
enum class EHelixAttackType : uint8
{
	Special_01	UMETA(DisplayName = "Special_01"),
	Special_02	UMETA(DisplayName = "Special_02"),
	Special_03	UMETA(DisplayName = "Special_03"),
	Cast		UMETA(DisplayName = "Cast"),
};

UENUM(BlueprintType)
enum class EPortalType : uint8
{
	None	UMETA(DisplayName = "None"),
	Boss	UMETA(DisplayName = "Boss"),
	Mission	UMETA(DisplayName = "Mission"),
};
