// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STGameInstance.generated.h"

/**
 * 
 */
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
};
