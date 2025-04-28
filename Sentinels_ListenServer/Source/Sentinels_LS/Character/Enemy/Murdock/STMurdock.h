// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/STEnemyBase.h"
#include "STMurdock.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API ASTMurdock : public ASTEnemyBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(Server, Reliable)
	void ActivateUltimate_Server();

protected:
	UFUNCTION(NetMulticast, Reliable)
	void ActivateUltimate_Multicast();

	void PlayUltimateMontage();

	UFUNCTION()
	void UltimateFire();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	UAnimMontage* Montage_Ultimate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AProjectileBase> SubclassOfUltimateProjectile;
};
