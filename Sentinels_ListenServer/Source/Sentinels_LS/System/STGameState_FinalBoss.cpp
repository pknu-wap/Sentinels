// Fill out your copyright notice in the Description page of Project Settings.


#include "System/STGameState_FinalBoss.h"
#include "Net/UnrealNetwork.h"
#include "SubSystem/STGameTravelDataSubsystem.h"

ASTGameState_FinalBoss::ASTGameState_FinalBoss() :
	LevelTag(FSTGameplayTags::Get().Level_Boss)
{
	bReplicates = true;
}

void ASTGameState_FinalBoss::BeginPlay()
{
	Super::BeginPlay();
}

void ASTGameState_FinalBoss::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASTGameState_FinalBoss, LevelTag);
}

void ASTGameState_FinalBoss::OnBossCleared_Multicast_Implementation()
{
	USTGameTravelDataSubsystem* gameTravelDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USTGameTravelDataSubsystem>();
	gameTravelDataSubsystem->SetLevelTag(FSTGameplayTags::Get().Level_Lobby);

	Delegate_OnBossCleared.Broadcast();
}
