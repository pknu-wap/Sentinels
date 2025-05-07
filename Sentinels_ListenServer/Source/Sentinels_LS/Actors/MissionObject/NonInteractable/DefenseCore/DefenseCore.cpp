// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MissionObject/NonInteractable/DefenseCore/DefenseCore.h"
#include "Components/BoxComponent.h"
#include "Components/SpawnEnemyComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/STPlayerCharacter.h"

ADefenseCore::ADefenseCore()
{
	TriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("TriggerBox"));
	TriggerBoxComponent->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);

	SpawnEnemyComp = CreateDefaultSubobject<USpawnEnemyComponent>(FName("SpawnComp"));
}

void ADefenseCore::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		TriggerBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADefenseCore::TriggerBoxBeginOverlap);
	}
}

void ADefenseCore::StopSpawnEnemy()
{
	SpawnEnemyComp->StopSpawnEnemy();
}

void ADefenseCore::TriggerBoxBeginOverlap
(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() && OtherActor && OtherActor->IsA<ASTPlayerCharacter>())
	{
		SpawnEnemyComp->StartSpawnEnemy();
	}
}

