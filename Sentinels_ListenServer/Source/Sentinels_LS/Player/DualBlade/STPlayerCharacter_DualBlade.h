// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/STPlayerCharacter.h"
#include "STPlayerCharacter_DualBlade.generated.h"



UCLASS()
class SENTINELS_LS_API ASTPlayerCharacter_DualBlade : public ASTPlayerCharacter
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

/*
	Damage (Server)
*/
public:
	virtual void AdjustFinalDamage(float& DamageAmount, FDamageEvent const& DamageEvent, AActor* DamagedActor) override;

/*
	On Attack Success (Passive Skill)
*/
public:
	virtual void OnAttackSuccess_Server_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UFUNCTION()
	void OnRep_CurrentComboStack();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxComboStack = 50;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentComboStack, VisibleAnywhere)
	int CurrentComboStack = 0;
};
