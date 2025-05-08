// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Section/STMissionSection.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASTMissionSection::ASTMissionSection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASTMissionSection::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASTMissionSection::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASTMissionSection, TagContainer);
}
