// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STEnemyStatusComponent.h"
#include "System/STGameState.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USTEnemyStatusComponent::USTEnemyStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	StatusCurve_Mission = LoadObject<UCurveFloat>(this, TEXT("/Script/Engine.CurveFloat'/Game/Sentinels/Enemy/Curve/Curve_AI_Status_Mission.Curve_AI_Status_Mission'"));
	StatusCurve_Time = LoadObject<UCurveFloat>(this, TEXT("/Script/Engine.CurveFloat'/Game/Sentinels/Enemy/Curve/Curve_AI_Status_Time.Curve_AI_Status_Time'"));
}


// Called when the game starts
void USTEnemyStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	StatusCurve_Current = StatusCurve_Mission;

	InitStatus();
}

void USTEnemyStatusComponent::InitStatus()
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		float weight = GetStatusCurveValue();

		CurrentHP = MaxHP * weight;
		CurrentATK = BaseATK * weight;

		bIsDied = false;
	}
}

float USTEnemyStatusComponent::GetStatusCurveValue() const
{
	if (!StatusCurve_Current) return 1.f;

	ASTGameState* GameState = Cast<ASTGameState>(UGameplayStatics::GetGameState(this));
	if (GameState)
	{
		if (StatusCurve_Current == StatusCurve_Mission)
		{
			return StatusCurve_Current->GetFloatValue(GameState->GetClearedMissionNum());
		}
		else if (StatusCurve_Current == StatusCurve_Time)
		{
			float minTime, maxTime;
			StatusCurve_Current->GetTimeRange(minTime, maxTime);

			float TimeSeconds = UGameplayStatics::GetTimeSeconds(this);
			return StatusCurve_Current->GetFloatValue(FMath::Clamp(TimeSeconds, 0.f, maxTime));
		}
	}

	return 1.f;
}

float USTEnemyStatusComponent::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHP = FMath::Clamp(CurrentHP - Damage, 0, MaxHP);

	if (CurrentHP <= 0)
		bIsDied = true;

	return CurrentHP;
}
