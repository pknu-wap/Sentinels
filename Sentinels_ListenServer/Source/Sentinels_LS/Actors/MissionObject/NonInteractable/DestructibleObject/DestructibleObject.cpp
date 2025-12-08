// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MissionObject/NonInteractable/DestructibleObject/DestructibleObject.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemObjects.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

ADestructibleObject::ADestructibleObject()
{
	GCComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(FName("GCComponent"));
	GCComponent->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	GCComponent->SetNotifyBreaks(true);
	GCComponent->SetNotifyGlobalBreaks(true);
	GCComponent->SetEnableDamageFromCollision(false);

	TArray<FName> ProfileNames;
	ProfileNames.Add(FName()); ProfileNames.Add(FName("Debris")); ProfileNames.Add(FName("Debris"));
	GCComponent->SetPerLevelCollisionProfileNames(ProfileNames);

	FieldComponent = CreateDefaultSubobject<UFieldSystemComponent>(FName("FieldComp"));
	FieldComponent->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);

	RadialFallOffComponent = CreateDefaultSubobject<URadialFalloff>(FName("RadialFallOffComponent"));

	SphereComponent = CreateDefaultSubobject<USphereComponent>(FName("Sphere"));
	SphereComponent->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
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

	// Set Damage Threshold
	TArray<float> DamageThreshold;
	for (int i = 0; i < GCComponent->DamageThreshold.Num(); i++)
	{
		DamageThreshold.Push(MaxHealth);
	}
	GCComponent->SetDamageThreshold(DamageThreshold);
}

float ADestructibleObject::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0, MaxHealth);
	if (CurrentHealth <= 0.f)
	{
		// Disable Collision With Player
		GCComponent->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
		GCComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);

		bIsSuccessed = true;

		// Start Fracture
		GetWorldTimerManager().SetTimerForNextTick(this, &ADestructibleObject::StartFracture);
		// StartFracture();
	}

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

	// Start Fade Away & Destroy When Ended
	StartFadeAway();

	/*
		Update Mission Info
	*/
	if (HasAuthority())
	{
		Delegate_MissionConditionUpdate.Broadcast(ObjectID, true);
	}
}
