// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectile/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Use a sphere as a simple collision representation.
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    // Set the sphere's collision radius.
    CollisionComponent->InitSphereRadius(15.0f);
    // Set the root component to be the collision component.
    RootComponent = CollisionComponent;

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

    SetLifeSpan(LifeTime);
}

void AProjectileBase::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

//void AProjectileBase::SetHomingTarget(AActor* InTarget)
//{
//    if (!IsValid(InTarget)) return;
//
//    USceneComponent* TargetSceneComp = InTarget->GetComponentByClass<USceneComponent>();
//
//    ProjectileMovementComponent->HomingTargetComponent = TargetSceneComp;
//    ProjectileMovementComponent->bIsHomingProjectile = true;
//    ProjectileMovementComponent->HomingAccelerationMagnitude = 25000.f;
//}
