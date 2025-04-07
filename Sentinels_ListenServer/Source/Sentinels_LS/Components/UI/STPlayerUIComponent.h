// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/STUIComponent_Base.h"
#include "STEnums.h"
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
	const TArray<FUniqueNetIdRepl>& GetPlayerID() const { return PlayerID; }

protected:
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCSetbIsReady(bool Value);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCChangebIsReady(FGameplayTag WidgetTag);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientRPCUnRegisterWidget(FGameplayTag WidgetTag);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCRegisterPlayerID(const FUniqueNetIdRepl& ID);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCUnRegisterPlayerID(const FUniqueNetIdRepl& ID);

public:
	UFUNCTION(BlueprintCallable)
	void UpdateLoadoutUI();

	UFUNCTION(BlueprintCallable)
	void UpdateCharacterSelectUI();

	UFUNCTION(BlueprintCallable)
	void AddPlayerID(const FUniqueNetIdRepl& ID);

protected:
	void RegisterIDToDummyPlayer(const FUniqueNetIdRepl& ID);


protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Lobby", meta = (AllowPrivateAccess = "true"))
	bool bIsReady;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Lobby", meta = (AllowPrivateAccess = "true"))
	ESTClassType STClassType;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Lobby", meta = (AllowPrivateAccess = "true"))
	TArray<FUniqueNetIdRepl> PlayerID;
};
