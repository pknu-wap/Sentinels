// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/STCharacterBase.h"
#include "STEnemyBase.generated.h"

class USTEnemyStatusComponent;

UCLASS()
class SENTINELS_LS_API ASTEnemyBase : public ASTCharacterBase
{
	GENERATED_BODY()

public:
	ASTEnemyBase(const FObjectInitializer& object_initializer);

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Attack
	UFUNCTION(Server, Reliable)
	void ActivateNormalAttack_Server();

	UFUNCTION(NetMulticast, Reliable)
	void ActivateNormalAttack_Multicast();

	void PlayNormalAttackMontage();

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USTEnemyStatusComponent> StatusComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	UAnimMontage* Montage_NormalAttack;
};
