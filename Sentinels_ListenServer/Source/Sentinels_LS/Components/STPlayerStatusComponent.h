// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STPlayerStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENTINELS_LS_API USTPlayerStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTPlayerStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// < Function Call Order >
	// UInventoryComponent::AddItem_Server -> USTPlayerStatusComponent::CalculateStatus
	void CalculateStatus();

	UFUNCTION()
	void OnRep_HPUpdated();

public:
	// HP 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseMaxHP;

	UPROPERTY(ReplicatedUsing = OnRep_HPUpdated, EditAnywhere, BlueprintReadOnly)
	float MaxHP;

	UPROPERTY(ReplicatedUsing = OnRep_HPUpdated, EditAnywhere, BlueprintReadOnly)
	float HP;

	// ATK & DEF
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	int ATK;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	int DEF;

	// Damage Increase Rate
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	float MaxDamageIncreaseRate;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	float DamageIncreaseRate;

	// CoolDown Reduction
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	float MaxCDR;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	float CDR;

	// Critical
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	float MaxCriticalRate;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	float CriticalRate;
};
