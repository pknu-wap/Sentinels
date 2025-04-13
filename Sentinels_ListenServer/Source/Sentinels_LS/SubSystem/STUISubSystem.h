// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "STGameplayTags.h"
#include "Blueprint/UserWidget.h"
#include "STUISubSystem.generated.h"

UCLASS()
class SENTINELS_LS_API USTUISubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	UFUNCTION(BlueprintCallable)
	void RegisterWidget(FGameplayTag WidgetTag, UUserWidget* Widget);

	UFUNCTION(BlueprintCallable)
	void UnRegisterWidget(FGameplayTag WidgetTag);

	UFUNCTION(BlueprintCallable)
	void SetWidgetVisibility(FGameplayTag WidgetTag, ESlateVisibility Visibility);

	UFUNCTION(BlueprintCallable)
	ESlateVisibility GetWidgetVisibility(FGameplayTag WidgetTag);

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetWidget(FGameplayTag WidgetTag);

private:
	TMap<FGameplayTag, UUserWidget*> WidgetMap;
};
