// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/STPlayerCharacter.h"
#include "STPlayerCharacter_Katana.generated.h"

class AProjectileBase;

UCLASS()
class SENTINELS_LS_API ASTPlayerCharacter_Katana : public ASTPlayerCharacter
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
protected:
	virtual void OnAttackSuccess_Server_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AActor* DamagedActor) override;

/*
	Passive Skill
*/
protected:
	virtual void Skill_Passive_Pressed() override;
	virtual void Skill_Passive_Pressed_Server_Implementation() override;

	UFUNCTION()
	void ApplyPassiveDamage();

	UFUNCTION()
	void SetWrapTarget_Passive();

protected:
	UPROPERTY(EditAnywhere, Category = "Skill")
	float TraceRange_Passive = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float TraceRadius_Passive = 100.f;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float WarpDistance_Passive = 500.f;

private:
	UPROPERTY()
	TArray<AActor*> AllTargetActors_Passive;

	UPROPERTY()
	AActor* TargetActor_Passive = nullptr;


/*
	Active Skill
*/
protected:
	UFUNCTION()
	void SetWrapTarget_Q();

	UFUNCTION()
	void SpawnSlash_Q();

protected:
	UPROPERTY(EditAnywhere, Category = "Skill")
	float WarpDistance_Q = 500.f;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AActor> SubclassOfSlash_Q;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AProjectileBase> SubclassOfSlash_R;

private:
	FVector SpawnLocation_Slash_Q;
};
