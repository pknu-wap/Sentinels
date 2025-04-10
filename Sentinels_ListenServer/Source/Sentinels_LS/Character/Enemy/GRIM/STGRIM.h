// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/STEnemyBase.h"
#include "STGRIM.generated.h"

class AProjectileBase;

UCLASS()
class SENTINELS_LS_API ASTGRIM : public ASTEnemyBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void PrimaryFire();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AProjectileBase> SubclassOfPrimaryProjectile;
};
