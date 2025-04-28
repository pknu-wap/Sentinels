// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/STCharacterBase.h"
#include "STEnemyBase.generated.h"

class USTEnemyStatusComponent;
class AProjectileBase;

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

private:
	FTimerHandle Handle_Stunned;

public:
	bool IsNormalAttackMontage(UAnimMontage* InMontage);

public:
	UFUNCTION(Server, Reliable)
	void StopCurrentAnimMontage_Multicast();

/*
	Attack
*/
public:
	int GetRandomNormalAttackMontageIndex();

	UFUNCTION(Server, Reliable)
	void ActivateNormalAttack_Server();

	UFUNCTION(NetMulticast, Reliable)
	void ActivateNormalAttack_Multicast(int MontageIdx);

	void PlayNormalAttackMontage(int MontageIdx);

/*
	Primary Fire	
*/
protected:
	UFUNCTION()
	virtual void PrimaryFire();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AProjectileBase> ProjectileClass_PrimaryFire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SocketName_PrimaryFire;

/*
	Hit
*/
protected:
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
	TArray<UAnimMontage*> Montage_NormalAttackSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	UAnimMontage* Montage_Knockback;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	UAnimMontage* Montage_KnockDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	UAnimMontage* Montage_Died;
	
public:
	FOnEnemyDied Delegate_OnEnemyDied;

private:
	int LastNormalAttackMontageIndex;
};
