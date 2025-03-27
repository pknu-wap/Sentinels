// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interact/DominationCore/InteractableDominationCore.h"
#include "Components/BoxComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/Enemy/STEnemyBase_AIController.h"
#include "Components/SpawnEnemyComponent.h"
#include "Player/STPlayerCharacter.h"

AInteractableDominationCore::AInteractableDominationCore()
{
	TriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("TriggerBox"));
	SpawnEnemyComp = CreateDefaultSubobject<USpawnEnemyComponent>(FName("SpawnComp"));
}

void AInteractableDominationCore::BeginPlay()
{
	if (HasAuthority())
	{
		TriggerBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AInteractableDominationCore::TriggerBoxBeginOverlap);
	}
}

void AInteractableDominationCore::TriggerBoxBeginOverlap
	(UPrimitiveComponent* OverlappedComponent,  AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex,  bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() && OtherActor && OtherActor->IsA<ASTPlayerCharacter>())
	{
		SpawnEnemyComp->StartSpawnEnemy();
	}
}
