// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STEnemyStatusComponent.h"
#include "System/STGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USTEnemyStatusComponent::USTEnemyStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UCurveFloat> OF_StatusCurve_Mission(TEXT("/Script/Engine.CurveFloat'/Game/Sentinels/Enemy/Curve/Curve_AI_Status_Mission.Curve_AI_Status_Mission'"));
	static ConstructorHelpers::FObjectFinder<UCurveFloat> OF_StatusCurve_Time(TEXT("/Script/Engine.CurveFloat'/Game/Sentinels/Enemy/Curve/Curve_AI_Status_Time.Curve_AI_Status_Time'"));

	StatusCurve_Mission = OF_StatusCurve_Mission.Object;
	StatusCurve_Time = OF_StatusCurve_Time.Object;
	// StatusCurve_Mission = LoadObject<UCurveFloat>(this, TEXT("/Script/Engine.CurveFloat'/Game/Sentinels/Enemy/Curve/Curve_AI_Status_Mission.Curve_AI_Status_Mission'"));
	// StatusCurve_Time = LoadObject<UCurveFloat>(this, TEXT("/Script/Engine.CurveFloat'/Game/Sentinels/Enemy/Curve/Curve_AI_Status_Time.Curve_AI_Status_Time'"));

	bReplicateUsingRegisteredSubObjectList = true;
}


// Called when the game starts
void USTEnemyStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	StatusCurve_Current = StatusCurve_Time;

	FTimerHandle StatusHandle;
	GetWorld()->GetTimerManager().SetTimer(StatusHandle, 
		this, &USTEnemyStatusComponent::UpdateStatus,
		30.f, true);

	InitStatus();
}

void USTEnemyStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(USTEnemyStatusComponent, MaxHP);
	DOREPLIFETIME(USTEnemyStatusComponent, CurrentHP);
}

void USTEnemyStatusComponent::InitStatus()
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		float weight = GetStatusCurveValue();

		MaxHP = BaseMaxHP * weight;
		CurrentHP = MaxHP;
		CurrentATK = BaseATK * weight;

		bIsDied = false;
	}
}

void USTEnemyStatusComponent::UpdateStatus()
{
	float weight = GetStatusCurveValue();
	MaxHP = BaseMaxHP * weight;
	CurrentATK = BaseATK * weight;
}

float USTEnemyStatusComponent::GetStatusCurveValue() const
{
	if (!StatusCurve_Current) return 1.f;

	UWorld* world = GetWorld();
	if (!world) return 1.f;

	ASTGameState* GameState = Cast<ASTGameState>(world->GetGameState());
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
