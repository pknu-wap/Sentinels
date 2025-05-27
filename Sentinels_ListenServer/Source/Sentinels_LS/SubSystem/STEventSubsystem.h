// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "STEventSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventOccur, FGameplayTag, Tag);

UCLASS()
class SENTINELS_LS_API USTEventSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	USTEventSubsystem();

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnEventOccur Delegate_EventOccur;


};
