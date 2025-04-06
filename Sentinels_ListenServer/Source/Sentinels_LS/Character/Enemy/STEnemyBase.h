// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/STCharacterBase.h"
#include "STEnemyBase.generated.h"

class USTEnemyStatusComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDied, AActor*, DiedEnemy);

UCLASS()
class SENTINELS_LS_API ASTEnemyBase : public ASTCharacterBase
{
	GENERATED_BODY()

public:
	ASTEnemyBase(const FObjectInitializer& object_initializer);

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	// Attack
	UFUNCTION(Server, Reliable)
	void ActivateNormalAttack_Server();

	UFUNCTION(NetMulticast, Reliable)
	void ActivateNormalAttack_Multicast();

	void PlayNormalAttackMontage();

	// Hit
	UFUNCTION(NetMulticast, Reliable)
	void PlayKnockbackMontage_Multicast();

	void PlayKnockbackMontage();

	// Die
	UFUNCTION(NetMulticast, Reliable)
	void PlayDiedMontage_Multicast();

	void PlayDiedMontage();

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USTEnemyStatusComponent> StatusComponent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	UAnimMontage* Montage_NormalAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	UAnimMontage* Montage_Knockback;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	UAnimMontage* Montage_KnockDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	UAnimMontage* Montage_Died;
	
public:
	FOnEnemyDied Delegate_OnEnemyDied;
};
