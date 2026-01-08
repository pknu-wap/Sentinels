// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/EnhancementSubsystem.h"

void UEnhancementSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	EnhancementDB_Total = LoadObject<UDataTable>(this, TEXT("/Script/Engine.CompositeDataTable'/Game/Sentinels/Player/ETC/Enhance/DB_Enhancement_Total.DB_Enhancement_Total'"));
	if (EnhancementDB_Total == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventorySubsystem : Failed to Load EnhancementDB_Total !!! "));
	}

	TArray<FName> rowNames = EnhancementDB_Total->GetRowNames();
	for (const FName& RowName : rowNames)
	{
		FEnhancementInfo* info = EnhancementDB_Total->FindRow<FEnhancementInfo>(RowName, TEXT("FindRow"));
		if (info != nullptr)
		{
			EnhancementMap.Add(info->EnhancementTag, *info);
		}
	}
}

const FEnhancementInfo* UEnhancementSubsystem::GetEnhancementInfo(FGameplayTag InEnhancementtag) const
{
	return EnhancementMap.Find(InEnhancementtag);
}
