// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STEnemyStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENTINELS_LS_API USTEnemyStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTEnemyStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void InitStatus();
	
	UFUNCTION()
	void UpdateStatus();

public:
	float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

public:
	FORCEINLINE void ResetGuardGuage() { CurrentGuardGuage = MaxGuardGuage; }
	FORCEINLINE bool IsDied() { return bIsDied; }

public:
	FORCEINLINE float GetMaxHP() const { return MaxHP; }
	FORCEINLINE float GetCurrentHP() const { return CurrentHP; }
	FORCEINLINE float GetCurrentATK() const { return CurrentATK; }
	FORCEINLINE float GetCurrentGuardGuage() const { return CurrentGuardGuage; }
	float GetStatusCurveValue() const;

protected:
	UPROPERTY()
	UCurveFloat* StatusCurve_Current;

	UPROPERTY(EditAnywhere)
	UCurveFloat* StatusCurve_Time;

	UPROPERTY(EditAnywhere)
	UCurveFloat* StatusCurve_Mission;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseMaxHP = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseATK = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CurrentATK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxGuardGuage = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CurrentGuardGuage;

	UPROPERTY(BlueprintReadOnly)
	bool bIsDied = false;

};
