// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "STStructs.generated.h"

USTRUCT(BlueprintType)
struct SENTINELS_LS_API FDropInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float DropProbability = 0.1f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AInteractableItem> DropItemClass;
};

USTRUCT(BlueprintType)
struct FRegisterMissionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag MissionTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class USTMissionBase> MissionSubClass;
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillDescription;
};

USTRUCT(BlueprintType)
struct SENTINELS_LS_API FInvSlotStruct : public FTableRowBase
{
	GENERATED_BODY()

	FInvSlotStruct() {};
	FInvSlotStruct(int InItemID, int InQuantity, class UItemObject* InItemObject)
		: ItemID(InItemID), Quantity(InQuantity), ItemObject(InItemObject)
	{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ItemID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Quantity = 0;

	UPROPERTY()
	class UItemObject* ItemObject;
};

USTRUCT(BlueprintType)
struct SENTINELS_LS_API FItemStruct : public FTableRowBase
{
	GENERATED_BODY()

	FItemStruct() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ItemID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UItemObject> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;
};

USTRUCT(BlueprintType)
struct SENTINELS_LS_API FCustomizeStruct : public FTableRowBase
{
	GENERATED_BODY()

	FCustomizeStruct() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;
};