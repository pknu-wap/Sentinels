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