// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/STBossBase.h"
#include "STEnums.h"
#include "STHelix.generated.h"

class AProjectileBase;

UCLASS()
class SENTINELS_LS_API ASTHelix : public ASTBossBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	void HelixAttack(EHelixAttackType AttackType);


	/*
		Special 01
	*/
public:
	void StartSpawnLazers();

	UFUNCTION(BlueprintCallable)
	void SpawnLazers();

protected:
	FTimerHandle Handle_Lazer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> LazerActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxLazerCount;

	int CurrentLazerCount;



	/*
		Special 02
	*/
public:
	void StartSpawnProjectiles();

protected:
	UFUNCTION()
	void SpawnProjectile();

protected:
	FTimerHandle Handle_Projectiles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxSpawnProjectiles;

	int SpawnedProjectiles;


	/*
		Special 03
	*/
public:
	void SpawnShockWave();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> ShockWaveActorClass;


	/*
		Cast
	*/
public:
	void StartSpawnExplosions();

protected:
	UFUNCTION()
	void SpawnExplosion();

protected:
	FTimerHandle Handle_Explosion;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> ExplosionClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxSpawnExplosions;

	int SpawnedExplosions;


protected:
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* AM_Special_01;
	
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* AM_Special_02;

	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* AM_Special_03;

	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* AM_Cast;
};
