// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STEnums.h"
#include "STPlayerStatusComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPDelegate, float, Current, float, Max);

USTRUCT(BlueprintType)
struct SENTINELS_LS_API FSTBuffStruct : public FTableRowBase
{
	GENERATED_BODY()

	FSTBuffStruct() {};
	
	FSTBuffStruct(ESTBuffType inType, float inDuration, float inValue)
		: BuffType(inType), Duration(inDuration), Value(inValue)
	{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ESTBuffType BuffType;

	UPROPERTY(VisibleAnywhere)
	float Duration;

	UPROPERTY(VisibleAnywhere)
	float Value;

	FTimerHandle Handle_Buff;
};

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
	ESTStatusType GetRelatedStatusType(ESTBuffType inBuffType) const;
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SetStatus_Server(ESTStatusType inType, float inValue, bool forceApply);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ApplyBuff_Server(ESTBuffType inType, float inValue, float inDuration);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ClearBuff_Server(ESTBuffType inType);

	/*
		< Function Call Order >
		1. Get Item! -> UInventoryComponent::AddItem_Server -> USTPlayerStatusComponent::CalculateStatus
		2. Get Buff! -> USTPlayerStatusComponent::ApplyBuff_Server -> USTPlayerStatusComponent::CalculateStatus
	*/ 
	UFUNCTION(BlueprintCallable)
	void SetDefaultStatus();

	UFUNCTION(BlueprintCallable)
	void CalculateStatus();

	UFUNCTION(BlueprintCallable)
	void ApplyStatus();

	UFUNCTION()
	void OnRep_HPUpdated();

protected:
	UPROPERTY(VisibleAnywhere)
	TMap<ESTBuffType, FSTBuffStruct> BuffMap;

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

	/*
		Movement Speed (MS)

		Calculation formula : MovementSpeed = BaseMovementSpeed * (1 + Item_MS) * (1 + Buff_MS)
			eg) 939 = 600 * (1 + 0.2) * (1 + 0.3)
	*/
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	float MaxMovementSpeed;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	float MovementSpeed;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	float BaseMovementSpeed;

	/*
		Attack Speed (AS)

		Calculation formula : AttackSpeed = BaseAttackSpeed * (1 + Item_AS) * (1 + Buff_AS)
			eg) 1.43 = 1 * (1 + 0.1) * (1 + 0.3)
	*/
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	float MaxAttackSpeed;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	float AttackSpeed;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	float BaseAttackSpeed;

	/*
		Damage Increase Rate (DIR)

		Calculation formula : DIR = Item_DIR + Buff_DIR
	*/
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	float MaxDamageIncreaseRate;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	float DamageIncreaseRate;

	/*
		CoolDown Reduction (CDR)

		Calculation formula : CDR = Item_CDR * Buff_CDR
			eg) 
			
		CoolTime formula : CoolTime = CoolTime * (1 - CDR);
	*/
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	float MaxCDR;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	float CDR;

	/*
		Critical Damage Percent (CDP)
		
		Calculation formula : CDP = BaseCDP * Item_CDP * Buff_CDP
			eg) 5.184 = 2.0f * (1.2 * 1.2 * 1.2) * (1.5)

		BaseCDP : 2.0f
	*/
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	float MaxCriticalDamagePercent;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)		//
	float CriticalDamagePercent;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	float BaseCriticalDamagePercent;

	/*
		Critical Rate (CR)

		Calculation formula : CR = BaseCR + Item_CR + Buff_CR 
			eg) 0.8 = 0.1 + 0.2 + 0.5
	*/
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	float MaxCriticalRate;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	float CriticalRate;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	float BaseCriticalRate;

public:
	UPROPERTY(BlueprintAssignable)
	FOnHPDelegate OnHPDelegate;
};
