// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MissionObject/NonInteractable/DestructibleObject/DestructibleObject.h"

ADestructibleObject::ADestructibleObject()
{
	GCComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(FName("GCComponent"));
	GCComponent->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	GCComponent->SetNotifyBreaks(true);
	GCComponent->SetNotifyGlobalBreaks(true);
}

void ADestructibleObject::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	GCComponent->OnChaosBreakEvent.AddDynamic(this, &ADestructibleObject::OnGCBreaked);

	/*
		It's for Show Outline overlay When This Object is Damaged;
		Show Mesh & Hide GC (When BeginPlay)
	*/
	Mesh->SetHiddenInGame(false);
	GCComponent->SetHiddenInGame(true);
}

float ADestructibleObject::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	/*
		Set Overlay Material on Mesh for a moment
	*/
	Mesh->SetOverlayMaterial(MI_Outline);
	GetWorldTimerManager().SetTimer(Handle_Outline, [&]() {Mesh->SetOverlayMaterial(nullptr); }, 0.1f, false);

	return 0.0f;
}

void ADestructibleObject::OnGCBreaked(const FChaosBreakEvent& BreakEvent)
{
	/*
		It's for Show Outline overlay When This Object is Damaged;
		Show GC & Hide Mesh (When Breaked)
	*/
	Mesh->SetHiddenInGame(true);
	GCComponent->SetHiddenInGame(false);
	
	// Disable Collision With Player
	GCComponent->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GCComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);

	// Start Fade Away

}
