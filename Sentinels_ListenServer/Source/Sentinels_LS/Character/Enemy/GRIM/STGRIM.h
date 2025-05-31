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

public:
	UFUNCTION(Server, Reliable)
	void ActivateUltimate_Server();

	UFUNCTION(NetMulticast, Reliable)
	void ActivateUltimate_Multicast();

	void PlayUltimateMontage();

protected:
	UFUNCTION()
	void UltimateFire();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AProjectileBase> SubclassOfPrimaryProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AProjectileBase> SubclassOfUltimateProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> SubclassOfTargetActor;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	UAnimMontage* Montage_Ultimate;

};
