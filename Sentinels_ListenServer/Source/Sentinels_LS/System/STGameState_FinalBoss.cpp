// Fill out your copyright notice in the Description page of Project Settings.


#include "System/STGameState_FinalBoss.h"
#include "STGameState_FinalBoss.h"

void ASTGameState_FinalBoss::BeginPlay()
{
	Super::BeginPlay();
}

void ASTGameState_FinalBoss::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ASTGameState_FinalBoss::OnBossCleared_Multicast_Implementation()
{
	Delegate_OnBossCleared.Broadcast();
}
