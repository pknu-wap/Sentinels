// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STEnemyStatusComponent.h"

// Sets default values for this component's properties
USTEnemyStatusComponent::USTEnemyStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USTEnemyStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	InitStatus();
}

void USTEnemyStatusComponent::InitStatus()
{
	CurrentHP = MaxHP;
	bIsDied = false;
}

float USTEnemyStatusComponent::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHP = FMath::Clamp(CurrentHP - Damage, 0, MaxHP);

	if (CurrentHP <= 0)
		bIsDied = true;

	return CurrentHP;
}

