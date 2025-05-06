// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DualBlade/STPlayerCharacter_DualBlade.h"
#include "Net/UnrealNetwork.h"

void ASTPlayerCharacter_DualBlade::BeginPlay()
{
	Super::BeginPlay();
}

void ASTPlayerCharacter_DualBlade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASTPlayerCharacter_DualBlade, CurrentComboStack, COND_OwnerOnly);
}

void ASTPlayerCharacter_DualBlade::AdjustFinalDamage(float& DamageAmount, FDamageEvent const& DamageEvent, AActor* DamagedActor)
{
	Super::AdjustFinalDamage(DamageAmount, DamageEvent, DamagedActor);

	float comboRate = (float) CurrentComboStack / MaxComboStack;
	DamageAmount = DamageAmount * (1 + comboRate);
}

void ASTPlayerCharacter_DualBlade::OnAttackSuccess_Server_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::OnAttackSuccess_Server_Implementation(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentComboStack = FMath::Clamp(CurrentComboStack + 1, 0, MaxComboStack);
}

void ASTPlayerCharacter_DualBlade::OnRep_CurrentComboStack()
{
}
