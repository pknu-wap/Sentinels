// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "STStructs.generated.h"

USTRUCT(BlueprintType)
struct FStatusEntry
{
	GENERATED_BODY()

	FStatusEntry()
	{
	}

	FStatusEntry(const FGameplayTag& inTag, int inCount) : StatusTag(inTag), Count(inCount)
	{
	}

	bool operator == (const FStatusEntry& inEntry) const
	{
		return StatusTag == inEntry.StatusTag;
	}

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag StatusTag;

	UPROPERTY(BlueprintReadWrite)
	int32 Count;
};

USTRUCT(BlueprintType)
struct SENTINELS_LS_API FEnhancementInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EnhancementTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UEnhancementObject> EnhancmenetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
};

USTRUCT(BlueprintType)
struct SENTINELS_LS_API FDropInfo : public FTableRowBase
{
	GENERATED_BODY()

	FDropInfo() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DropQuantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropProbability = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
		SkillName = other.SkillName;
		SkillDescription = other.SkillDescription;
	}

	FSkillStruct& operator=(const FSkillStruct& other)
	{
		ID = other.ID;
		Icon = other.Icon;
		SkillCoolTime = other.SkillCoolTime;
		SkillName = other.SkillName;
		SkillDescription = other.SkillDescription;

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

	FItemStruct(const FItemStruct& other)
	{
		ItemID = other.ItemID;
		ItemClass = other.ItemClass;
		Icon = other.Icon;
		ItemName = other.ItemName;
		ItemDescription = other.ItemDescription;
	}

	FItemStruct& operator=(const FItemStruct& other)
	{
		ItemID = other.ItemID;
		ItemClass = other.ItemClass;
		Icon = other.Icon;
		ItemName = other.ItemName;
		ItemDescription = other.ItemDescription;

		return *this;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ItemID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UItemObject> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemDescription;
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

USTRUCT(BlueprintType)
struct SENTINELS_LS_API FMapInfoStruct : public FTableRowBase
{
	GENERATED_BODY()

	FMapInfoStruct() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag LevelTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* MapImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MapDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MapSize = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Zoom = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitX = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitY = 0.0f;
};

USTRUCT(BlueprintType)
struct SENTINELS_LS_API FCharacterStateStruct : public FTableRowBase
{
	GENERATED_BODY()

	FCharacterStateStruct() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag StateTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;
};

USTRUCT(BlueprintType)
struct SENTINELS_LS_API FPortalInfo : public FTableRowBase
{
	GENERATED_BODY()

	FPortalInfo() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActivated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ArrivalPoint;
};