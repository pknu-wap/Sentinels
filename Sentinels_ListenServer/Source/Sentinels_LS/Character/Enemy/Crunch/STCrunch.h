// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/STBossBase.h"
#include "STCrunch.generated.h"

class AProjectileBase;

UCLASS()
class SENTINELS_LS_API ASTCrunch : public ASTBossBase
{
	GENERATED_BODY()
	
public:
	void StartSpawnMissiles();

	void SetDashWarpTarget(const FName WarpTargetName, const FTransform InTransform);
	void StartDash();
	void EndDash();

private:
	void SpawnMissile();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* Montage_SpawnMissile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AProjectileBase> SubclassOfMissile;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxMissiles = 100;

private:
	FTimerHandle Handle_SpawnMissiles;

	UPROPERTY()
	int SpawnedMissiles = 0;
};
