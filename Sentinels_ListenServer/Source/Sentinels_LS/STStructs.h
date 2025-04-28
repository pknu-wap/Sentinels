// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "STStructs.generated.h"

USTRUCT(BlueprintType)
struct SENTINELS_LS_API FSkillStruct : public FTableRowBase
{
	GENERATED_BODY()
	FSkillStruct() {}

	FSkillStruct(const FSkillStruct& other)
	{
		ID = other.ID;
		Icon = other.Icon;
		SkillCoolTime = other.SkillCoolTime;
	}

	FSkillStruct& operator=(const FSkillStruct& other)
	{
		ID = other.ID;
		Icon = other.Icon;
		SkillCoolTime = other.SkillCoolTime;
		return *this;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillCoolTime = 5.f;
};

USTRUCT(BlueprintType)
struct SENTINELS_LS_API FInvSlotStruct : public FTableRowBase
{
	GENERATED_BODY()
	FInvSlotStruct() {};
	FInvSlotStruct(int InItemID, int InQuantity) : ItemID(InItemID), Quantity(InQuantity)
	{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ItemID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Quantity = 0;
};

USTRUCT(BlueprintType)
struct SENTINELS_LS_API FItemStruct : public FTableRowBase
{
	GENERATED_BODY()

	FItemStruct() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ItemID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DEF = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeedIncreaseRate = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeedIncreaseRate = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageIncreaseRate = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CDR = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalDamagePercent = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalRate = 0;
};