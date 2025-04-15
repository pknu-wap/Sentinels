// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class SENTINELS_LS_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void FireInDirection(const FVector& ShootDirection);

	UFUNCTION(BlueprintImplementableEvent)
	void SetTarget(AActor* InTarget);

	UFUNCTION(BlueprintImplementableEvent)
	void SetTargetLocation(FVector InTargetLocation);

	UFUNCTION(BlueprintImplementableEvent)
	void SetHomingTarget(AActor* InTarget);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	float LifeTime = 10.f;
};
