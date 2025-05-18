// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectile/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // Use a sphere as a simple collision representation.
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    // Set the sphere's collision radius.
    CollisionComponent->InitSphereRadius(15.0f);
    // Set the root component to be the collision component.
    CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    // Use this component to drive this projectile's movement.
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = 3000.0f;
    ProjectileMovementComponent->MaxSpeed = 3000.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = true;
    ProjectileMovementComponent->Bounciness = 0.3f;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

}

void AProjectileBase::Activate(const FVector ActivateLocation, const FRotator ActivateRotation)
{
    Super::Activate(ActivateLocation, ActivateRotation);

    UE_LOG(LogTemp, Display, TEXT("%s"), *GetActorLocation().ToString());

    CollisionComponent->SetRelativeLocation(FVector(0, 0, 0));
    CollisionComponent->Activate();
    ProjectileMovementComponent->Activate();

    FTimerHandle DeactivateHandle;
    GetWorldTimerManager().SetTimer(DeactivateHandle, this, &AProjectileBase::Deactivate, LifeTime, false);
}

void AProjectileBase::Deactivate()
{
    Super::Deactivate();

    CollisionComponent->Deactivate();
    ProjectileMovementComponent->Deactivate();
}

void AProjectileBase::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}