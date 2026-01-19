// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/STUIComponent_Base.h"
#include "STGameplayTags.h"
#include "STPlayerUIComponent.generated.h"

/**
 * 
 */

class UWidget;

UCLASS()
class SENTINELS_LS_API USTPlayerUIComponent : public USTUIComponent_Base
{
	GENERATED_BODY()

public:
	USTPlayerUIComponent();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	/*
		Blueprint Function
	*/
	UFUNCTION(BlueprintCallable, Category = "UI")
	static UWidget* GetWidgetByName(UUserWidget* WidgetInstance, const FString& Name);

public:
	/*
		RPC
	*/
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCRegisterWidget(FGameplayTag WidgetTag, TSubclassOf<UUserWidget> WidgetClass);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientRPCRegisterWidget(FGameplayTag WidgetTag, TSubclassOf<UUserWidget> WidgetClass);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientRPCUnRegisterWidget(FGameplayTag WidgetTag);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCUpdateUI(FGameplayTag WidgetTag);

	UFUNCTION(Client, UnReliable, BlueprintCallable)
	void ClientRPCUpdateUI(FGameplayTag WidgetTag);

	//

	UFUNCTION(Client, UnReliable, BlueprintCallable)
	void ClientRPCAddToViewport(FGameplayTag WidgetTag);

	UFUNCTION(Client, UnReliable, BlueprintCallable)
	void ClientRPCRemoveFromParent(FGameplayTag WidgetTag);

public:
	/*
		Local
	*/
	UFUNCTION(BlueprintCallable)
	void UpdateUI(FGameplayTag WidgetTag);

	UFUNCTION(BlueprintCallable)
	void RegisterWidget(FGameplayTag WidgetTag, UUserWidget* Widget);

	UFUNCTION(BlueprintCallable)
	void CreateAndRegisterWidget(FGameplayTag WidgetTag, TSubclassOf<UUserWidget> WidgetClass);

	UFUNCTION(BlueprintCallable)
	void UnRegisterWidget(FGameplayTag WidgetTag);

	UFUNCTION(BlueprintCallable)
	void AddToViewport(FGameplayTag WidgetTag);

	UFUNCTION(BlueprintCallable)
	void SetVisibility(FGameplayTag WidgetTag, ESlateVisibility Visibility);
};
