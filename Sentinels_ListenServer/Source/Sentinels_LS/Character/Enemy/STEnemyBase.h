// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/STPoolableCharacter.h"
#include "STStructs.h"
#include "STEnemyBase.generated.h"

class USTEnemyStatusComponent;
class AProjectileBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDied, AActor*, DiedEnemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyStateAdd, FGameplayTag, AddedState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyStateRemove, FGameplayTag, RemovedState);

UCLASS()
class SENTINELS_LS_API ASTEnemyBase : public ASTPoolableCharacter
{
	GENERATED_BODY()

public:
	ASTEnemyBase(const FObjectInitializer& object_initializer);

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	FTimerHandle Handle_Stunned;
	FTimerHandle Handle_Deactivate;

/*
	AI Logic
*/
public:
	virtual void Activate(const FVector ActivateLocation = FVector::ZeroVector, const FRotator ActivateRotation = FRotator::ZeroRotator) override;
	virtual void Deactivate() override;

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

	UFUNCTION(NetMulticast, Reliable)
	void PrimaryFire_Multicast();

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

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UWidgetComponent* WC_EnemyMain_Screen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USTWC_LocalPlayerFacing* WC_EnemyMain_World;

/*
	UI
*/
protected:
	UFUNCTION(NetMulticast, Reliable)
	void ShowDamageIndicateWidget_Multicast(float Damage, FLinearColor Color);

	void ShowDamageIndicateWidget(float Damage, FLinearColor Color);

	UFUNCTION(NetMulticast, Reliable)
	void UpdateEnemyStateWidget_Multicast(FGameplayTag StateTag, bool bShow);

/*
	Die
*/
public:
	bool IsAlive() const;

	UFUNCTION(NetMulticast, Reliable)
	void DissolveStart_Multicast();

	UFUNCTION(BlueprintImplementableEvent)
	void DissolveStart();

	UFUNCTION(NetMulticast, Reliable)
	void DissolveReverseStart_Multicast();

	UFUNCTION(BlueprintImplementableEvent)
	bool DissolveReverseStart();

	UFUNCTION(BlueprintCallable)
	void DissolveEnded();

	UFUNCTION(BlueprintCallable)
	void DissolveReverseEnded();

	UFUNCTION(NetMulticast, Reliable)
	void PlayDiedMontage_Multicast();

	void PlayDiedMontage();

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USTEnemyStatusComponent> StatusComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damaged")
	float LaunchVelocity = 1000.f;


/*
	Drop
*/
public:
	UFUNCTION(BLueprintCallable)
	void SetAdditionalDropInfos(const TArray<FDropInfo>& inDropInfos);

	UFUNCTION(BlueprintCallable)
	virtual void DropItem();

protected:
	UPROPERTY(EditAnywhere, Category = "Drop")
	FDropInfo DropInfo_Base;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Drop")
	TArray<FDropInfo> DropInfos_Additional;


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
	UPROPERTY(BlueprintAssignable)
	FOnEnemyDied Delegate_OnEnemyDied;

	UPROPERTY(BlueprintAssignable)
	FOnEnemyStateAdd Delegate_OnEnemyStateAdd;

	UPROPERTY(BlueprintAssignable)
	FOnEnemyStateRemove Delegate_OnEnemyStateRemove;

private:
	int LastNormalAttackMontageIndex;
};
