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
	float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

public:
	FORCEINLINE bool IsDied() { return bIsDied; }

public:
	FORCEINLINE float GetMaxHP() const { return MaxHP; }
	FORCEINLINE float GetCurrentHP() const { return CurrentHP; }

protected:
	UPROPERTY(EditAnywhere)
	float MaxHP = 500.f;

	UPROPERTY(EditAnywhere)
	float CurrentHP;

	UPROPERTY()
	bool bIsDied = false;

};
