// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STEnums.h"
#include "STGameplayTags.h"
#include "STGameInstance.generated.h"

/**
 * 
 */

USTRUCT()
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FUniqueNetIdRepl PlayerID = FUniqueNetIdRepl();

	UPROPERTY()
	ESTClassType PlayerClass = ESTClassType::GreatSword;

	// 플레이어 메시 파츠
};


UCLASS()
class SENTINELS_LS_API USTGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable)
	UGameInstanceSubsystem* GetSubsystem(TSubclassOf<UGameInstanceSubsystem> SubsystemClass) const
	{
		return UGameInstance::GetSubsystemBase(SubsystemClass);
	}

public:
	FName CurrentSessionName;

	TArray<FPlayerInfo> PlayerInfos;

	FGameplayTag CurrentLevelTag;
};
