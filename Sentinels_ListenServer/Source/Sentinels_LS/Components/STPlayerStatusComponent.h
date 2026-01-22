// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STEnums.h"
#include "DamageType/STDamageTypes.h"
#include "STGameplayTags.h"
#include "STPlayerStatusComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPDelegate, float, Current, float, Max);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEXPUpdatedDelegate, float, Current, float, Max);

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

USTRUCT(BlueprintType)
struct SENTINELS_LS_API FSTDamageInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bIsCritical = false;

	UPROPERTY(BlueprintReadOnly)
	float DamageAmount = 0.f;
};

class UUserWidget;
class UEnhancementObject;
class UClassStatusDataAsset;

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
	// Return HP after damaged
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

public:
	void InitializeStatus(ESTClassType InClassType);
	void CopyProperties(USTPlayerStatusComponent* InStatusComponent);

protected:
	void InitializeStatusWithAsset(UClassStatusDataAsset* InDataAsset);

protected:
	UPROPERTY()
	class ASTPlayerCharacter* CachedPlayer;

	UPROPERTY()
	class UInventoryComponent* CachedInventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UClassStatusDataAsset* GreatSwordDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UClassStatusDataAsset* KatanaDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UClassStatusDataAsset* DualBaldeDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UDamageType> BaseDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UDamageType> CriticalDamageType;

public:
	UFUNCTION(BlueprintCallable)
	void AddExp(float InExp);

	UFUNCTION(BlueprintCallable)
	FSTDamageInfo GetCalculatedDamageInfo(FSTPointDamageEvent& DamageEvent, AActor* DamagedActor);

	UFUNCTION(BlueprintCallable)
	float GetBaseDamage() const;

	UFUNCTION(BlueprintCallable)
	float GetCriticalBaseDamage() const;

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
		3. Get Enhancement! -> USTPlayerStatusComponent::EnhancementSelected_Server -> USTPlayerStatusComponent::CalculateStatus
	*/ 
	void InitializeEnhancement();

	UFUNCTION(BlueprintCallable)
	void InitializeDefaultStatus();

	UFUNCTION(BlueprintCallable)
	void UpdateDefaultStatus();

	UFUNCTION(BlueprintCallable)
	void CalculateStatus();

	UFUNCTION(BlueprintCallable)
	void ApplyStatus();

	UFUNCTION()
	void OnRep_HPUpdated();

	UFUNCTION()
	void OnRep_LevelUpdated();

	UFUNCTION()
	void OnRep_ExpUpdated();

	UFUNCTION()
	void OnRep_SelectedEnhancements();

protected:
	void RegenHP();

	UFUNCTION(Client, Reliable)
	void GiveEnhancementSelections_Client(const TArray<FGameplayTag>& enhancements);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void EnhancementSelected_Server(FGameplayTag SelectedEnhancement);

protected:
	UPROPERTY(VisibleAnywhere)
	TMap<ESTBuffType, FSTBuffStruct> BuffMap;

public:
	// Level & Exp
	UPROPERTY(Category = "Level", EditAnywhere, BlueprintReadOnly)
	UCurveFloat* ExpCurve;

	UPROPERTY(ReplicatedUsing = OnRep_LevelUpdated, Category = "Level", VisibleAnywhere, BlueprintReadOnly)
	int Level = 1;

	UPROPERTY(Category = "Level", VisibleAnywhere, BlueprintReadOnly)
	float MaxExp;

	UPROPERTY(ReplicatedUsing = OnRep_ExpUpdated, Category = "Level", VisibleAnywhere, BlueprintReadOnly)
	float Exp;

	// Enhancement
	UPROPERTY(Category = "Enhancement", EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> WidgetClass_EnhancementSelect;

	UPROPERTY(Category = "Enhancement", EditAnywhere, BlueprintReadOnly)
	UDataTable* DB_Enhancement;

	UPROPERTY(ReplicatedUsing = OnRep_SelectedEnhancements)
	TArray<UEnhancementObject*> SelectedEnhancements;

	UPROPERTY()
	TMap<FGameplayTag, int> NotSelectedEnhancements;

	// HP 
	UPROPERTY(Category = "HP", EditAnywhere, BlueprintReadOnly)
	UCurveFloat* MaxHPCurve;

	UPROPERTY(Category = "HP", EditAnywhere, BlueprintReadOnly)
	float BaseMaxHP;

	UPROPERTY(ReplicatedUsing = OnRep_HPUpdated, Category = "HP", EditAnywhere, BlueprintReadWrite)
	float MaxHP;

	UPROPERTY(ReplicatedUsing = OnRep_HPUpdated, Category = "HP", EditAnywhere, BlueprintReadOnly)
	float HP;


	// HP Regen
	UPROPERTY(Category = "HP", EditAnywhere, BlueprintReadOnly)
	UCurveFloat* HPRegenCurve;

	UPROPERTY(Category = "HP", EditAnywhere, BlueprintReadOnly)
	float BaseMaxHPRegen;

	UPROPERTY(Category = "HP", EditAnywhere, BlueprintReadOnly)
	float MaxHPRegen;

	UPROPERTY(Category = "HP", EditAnywhere, BlueprintReadOnly)
	float BaseHPRegen;

	UPROPERTY(Replicated, Category = "HP", EditAnywhere, BlueprintReadWrite)
	float HPRegen;


	// ATK & DEF
	UPROPERTY(Category = "Combat", EditAnywhere, BlueprintReadOnly)
	UCurveFloat* ATKCurve;
	UPROPERTY(Category = "Combat", EditAnywhere, BlueprintReadOnly)
	UCurveFloat* DEFCurve;

	UPROPERTY(Replicated, Category = "Combat", EditAnywhere, BlueprintReadWrite)
	int ATK;
	UPROPERTY(Replicated, Category = "Combat", EditAnywhere, BlueprintReadWrite)
	int BaseATK;
	UPROPERTY(Replicated, Category = "Combat", EditAnywhere, BlueprintReadWrite)
	int DEF;
	UPROPERTY(Replicated, Category = "Combat", EditAnywhere, BlueprintReadWrite)
	int BaseDEF;

	/*
		Movement Speed (MS)

		Calculation formula : MovementSpeed = BaseMovementSpeed * (1 + Item_MS) * (1 + Buff_MS)
			eg) 939 = 600 * (1 + 0.2) * (1 + 0.3)
	*/
	UPROPERTY(Category = "Speed", EditAnywhere, BlueprintReadOnly)
	UCurveFloat* MoveSpeedCurve;
	UPROPERTY(Replicated, Category = "Speed", EditAnywhere, BlueprintReadOnly)
	float MaxMovementSpeed;
	UPROPERTY(Replicated, Category = "Speed", VisibleAnywhere, BlueprintReadWrite)
	float MovementSpeed;
	UPROPERTY(Replicated, Category = "Speed", EditAnywhere, BlueprintReadOnly)
	float BaseMovementSpeed;

	/*
		Attack Speed (AS)

		Calculation formula : AttackSpeed = BaseAttackSpeed * (1 + Item_AS) * (1 + Buff_AS)
			eg) 1.43 = 1 * (1 + 0.1) * (1 + 0.3)
	*/
	UPROPERTY(Category = "Combat", EditAnywhere, BlueprintReadOnly)
	UCurveFloat* AttackSpeedCurve;
	UPROPERTY(Replicated, Category = "Combat", EditAnywhere, BlueprintReadOnly)
	float MaxAttackSpeed;
	UPROPERTY(Replicated, Category = "Combat", VisibleAnywhere, BlueprintReadWrite)
	float AttackSpeed;
	UPROPERTY(Replicated, Category = "Combat", EditAnywhere, BlueprintReadOnly)
	float BaseAttackSpeed;

	/*
		Damage Increase Rate (DIR)

		Calculation formula : DIR = Item_DIR + Buff_DIR
	*/
	UPROPERTY(Replicated, Category = "Combat", EditAnywhere, BlueprintReadOnly)
	float MaxDamageIncreaseRate;
	UPROPERTY(Replicated, Category = "Combat", VisibleAnywhere, BlueprintReadWrite)
	float DamageIncreaseRate;

	/*
		CoolDown Reduction (CDR)

		Calculation formula : CDR = Item_CDR * Buff_CDR
			eg) 
			
		CoolTime formula : CoolTime = CoolTime * (1 - CDR);
	*/
	UPROPERTY(Replicated, Category = "Combat", EditAnywhere, BlueprintReadOnly)
	float MaxCDR;
	UPROPERTY(Replicated, Category = "Combat", VisibleAnywhere, BlueprintReadWrite)
	float CDR;

	/*
		Critical Damage Percent (CDP)
		
		Calculation formula : CDP = BaseCDP + Item_CDP + Buff_CDP
			eg) 5.184 = 2.0f + (0.2 + 0.2 + 0.2) + (0.5)

		BaseCDP : 2.0f
	*/
	UPROPERTY(Replicated, Category = "Combat", EditAnywhere, BlueprintReadOnly)
	float MaxCriticalDamagePercent;
	UPROPERTY(Replicated, Category = "Combat", VisibleAnywhere, BlueprintReadWrite)		//
	float CriticalDamagePercent;
	UPROPERTY(Replicated, Category = "Combat", EditAnywhere, BlueprintReadOnly)
	float BaseCriticalDamagePercent;

	/*
		Critical Rate (CR)

		Calculation formula : CR = BaseCR + Item_CR + Buff_CR 
			eg) 0.8 = 0.1 + 0.2 + 0.5
	*/
	UPROPERTY(Replicated, Category = "Combat", EditAnywhere, BlueprintReadOnly)
	float MaxCriticalRate;
	UPROPERTY(Replicated, Category = "Combat", VisibleAnywhere, BlueprintReadWrite)
	float CriticalRate;
	UPROPERTY(Replicated, Category = "Combat", EditAnywhere, BlueprintReadOnly)
	float BaseCriticalRate;

public:
	UPROPERTY(BlueprintAssignable)
	FOnHPDelegate OnHPDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnEXPUpdatedDelegate OnEXPUpdatedDelegate;
};
