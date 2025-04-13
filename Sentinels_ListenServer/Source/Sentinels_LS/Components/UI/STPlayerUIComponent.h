// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/STUIComponent_Base.h"
#include "STGameplayTags.h"
#include "STPlayerUIComponent.generated.h"

/**
 * 
 */

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
		Debug
	*/

	UFUNCTION(BlueprintCallable)
	FString GetPlayerIDString(const FUniqueNetIdRepl& ID) { return *ID.ToString(); }

public:
	UFUNCTION(BlueprintCallable)
	const bool GetbIsReady() const { return bIsReady; }

	UFUNCTION(BlueprintCallable)
	const FGameplayTag GetCurrentLevelTag();

	UFUNCTION(BlueprintCallable)
	void SetCurrentLevelTag(FGameplayTag LevelTag);

	UFUNCTION(BlueprintCallable)
	const TArray<FUniqueNetIdRepl>& GetPlayerID() const { return PlayerID; }

public:
	/*
		RPC
	*/
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientRPCRegisterWidget(FGameplayTag WidgetTag, TSubclassOf<UUserWidget> WidgetClass);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientRPCUnRegisterWidget(FGameplayTag WidgetTag);

	UFUNCTION(Client, UnReliable, BlueprintCallable)
	void ClientRPCAddToViewport(FGameplayTag WidgetTag);

	UFUNCTION(Client, UnReliable, BlueprintCallable)
	void ClientRPCRemoveFromParent(FGameplayTag WidgetTag);

	UFUNCTION(Client, UnReliable, BlueprintCallable)
	void ClientRPCUpdateUI(FGameplayTag WidgetTag);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCUpdateUI(FGameplayTag WidgetTag);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCRegisterPlayerID(const FUniqueNetIdRepl& ID);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCUnRegisterPlayerID(const FUniqueNetIdRepl& ID);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCSetbIsReady(bool Value);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCCheckbIsReady(FGameplayTag WidgetTag);

public:
	/*
		Local
	*/
	UFUNCTION(BlueprintCallable)
	void UpdateLoadoutUI();

	UFUNCTION(BlueprintCallable)
	void UpdateCurrentGameLevelUI();

	UFUNCTION(BlueprintCallable)
	void UpdateCharacterSelectUI();

	/*
		Server
	*/
	UFUNCTION(BlueprintCallable)
	void AddPlayerID(const FUniqueNetIdRepl& ID);

protected:
	/*
		Local
	*/
	void RegisterIDToDummyPlayer(const FUniqueNetIdRepl& ID);

	/*
		Server
	*/

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Lobby", meta = (AllowPrivateAccess = "true"))
	bool bIsReady;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Lobby", meta = (AllowPrivateAccess = "true"))
	TArray<FUniqueNetIdRepl> PlayerID;
};
