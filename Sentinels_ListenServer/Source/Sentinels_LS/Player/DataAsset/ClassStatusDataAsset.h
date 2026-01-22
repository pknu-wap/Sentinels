// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ClassStatusDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API UClassStatusDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// Enhancement
	UPROPERTY(Category = "Enhancement", EditAnywhere, BlueprintReadOnly)
	UDataTable* DB_Enhancement;

	// Damage Type
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UDamageType> BaseDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UDamageType> CriticalDamageType;

	// HP 
	UPROPERTY(Category = "HP", EditAnywhere, BlueprintReadOnly)
	float BaseMaxHP;

	UPROPERTY(Category = "HP", EditAnywhere, BlueprintReadWrite)
	float MaxHP;

	// HP Regen
	UPROPERTY(Category = "HP", EditAnywhere, BlueprintReadOnly)
	float BaseMaxHPRegen;

	UPROPERTY(Category = "HP", EditAnywhere, BlueprintReadOnly)
	float MaxHPRegen;

	UPROPERTY(Category = "HP", EditAnywhere, BlueprintReadOnly)
	float BaseHPRegen;


	// ATK & DEF
	UPROPERTY(Category = "Combat", EditAnywhere, BlueprintReadWrite)
	int BaseATK;
	UPROPERTY(Category = "Combat", EditAnywhere, BlueprintReadWrite)
	int BaseDEF;

	/*
		Movement Speed (MS)

		Calculation formula : MovementSpeed = BaseMovementSpeed * (1 + Item_MS) * (1 + Buff_MS)
			eg) 939 = 600 * (1 + 0.2) * (1 + 0.3)
	*/
	UPROPERTY(Category = "Speed", EditAnywhere, BlueprintReadOnly)
	float MaxMovementSpeed;
	UPROPERTY(Category = "Speed", EditAnywhere, BlueprintReadOnly)
	float BaseMovementSpeed;

	/*
		Attack Speed (AS)

		Calculation formula : AttackSpeed = BaseAttackSpeed * (1 + Item_AS) * (1 + Buff_AS)
			eg) 1.43 = 1 * (1 + 0.1) * (1 + 0.3)
	*/
	UPROPERTY(Category = "Combat", EditAnywhere, BlueprintReadOnly)
	float MaxAttackSpeed;
	UPROPERTY(Category = "Combat", EditAnywhere, BlueprintReadOnly)
	float BaseAttackSpeed;

	/*
		Damage Increase Rate (DIR)

		Calculation formula : DIR = Item_DIR + Buff_DIR
	*/
	UPROPERTY(Category = "Combat", EditAnywhere, BlueprintReadOnly)
	float MaxDamageIncreaseRate;

	/*
		CoolDown Reduction (CDR)

		Calculation formula : CDR = Item_CDR * Buff_CDR
			eg)

		CoolTime formula : CoolTime = CoolTime * (1 - CDR);
	*/
	UPROPERTY(Category = "Combat", EditAnywhere, BlueprintReadOnly)
	float MaxCDR;

	/*
		Critical Damage Percent (CDP)

		Calculation formula : CDP = BaseCDP + Item_CDP + Buff_CDP
			eg) 5.184 = 2.0f + (0.2 + 0.2 + 0.2) + (0.5)

		BaseCDP : 2.0f
	*/
	UPROPERTY(Category = "Combat", EditAnywhere, BlueprintReadOnly)
	float MaxCriticalDamagePercent;
	UPROPERTY(Category = "Combat", EditAnywhere, BlueprintReadOnly)
	float BaseCriticalDamagePercent;

	/*
		Critical Rate (CR)

		Calculation formula : CR = BaseCR + Item_CR + Buff_CR
			eg) 0.8 = 0.1 + 0.2 + 0.5
	*/
	UPROPERTY(Category = "Combat", EditAnywhere, BlueprintReadOnly)
	float MaxCriticalRate;
	UPROPERTY(Category = "Combat", EditAnywhere, BlueprintReadOnly)
	float BaseCriticalRate;
};
