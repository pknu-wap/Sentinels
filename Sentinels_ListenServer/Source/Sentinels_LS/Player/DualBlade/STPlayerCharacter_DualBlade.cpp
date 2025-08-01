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

void ASTPlayerCharacter_DualBlade::OnAttackSuccess_Server_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AActor* DamagedActor)
{
	Super::OnAttackSuccess_Server_Implementation(DamageAmount, DamageEvent, DamagedActor);

	CurrentComboStack = FMath::Clamp(CurrentComboStack + 1, 0, MaxComboStack);
	UE_LOG(LogTemp, Warning, TEXT("CurrentComboStack : %d"), CurrentComboStack);
	OnRep_CurrentComboStack();
}

void ASTPlayerCharacter_DualBlade::OnRep_CurrentComboStack()
{
	if (CurrentComboStack >= 30)
	{
		OnPassiveSkillReadyStateChanged.Broadcast(true);
	}
	else
	{
		OnPassiveSkillReadyStateChanged.Broadcast(false);
	}
}

void ASTPlayerCharacter_DualBlade::Skill_Passive_Pressed()
{
	if (!CanDoSkill()) return;

	Skill_Passive_Pressed_Server();
}

void ASTPlayerCharacter_DualBlade::Skill_Passive_Pressed_Server_Implementation()
{
	if (CurrentComboStack >= 30)
	{
		CurrentComboStack = FMath::Clamp(CurrentComboStack - 30, 0, MaxComboStack);
		Skill_Passive_Pressed_Multicast();
		PlayMontage_Skill_Passive();
	}
}

void ASTPlayerCharacter_DualBlade::Skill_Passive_Pressed_Multicast_Implementation()
{
	PlayMontage_Skill_Passive();
}