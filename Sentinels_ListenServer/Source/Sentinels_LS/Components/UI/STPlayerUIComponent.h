// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/STUIComponent_Base.h"
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
	UFUNCTION(BlueprintCallable)
	const bool GetbIsReady() const { return bIsReady; }

protected:
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCSetbIsReady(bool Value);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCChangebIsReady();

	UFUNCTION(Client, UnReliable, BlueprintCallable)
	void ClientRPCCheckbIsReady();

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Lobby", meta = (AllowPrivateAccess = "true"))
	bool bIsReady;
	
};
