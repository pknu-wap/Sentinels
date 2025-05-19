// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/STPlayerCharacter.h"
#include "STPlayerCharacter_GreatSword.generated.h"


UCLASS()
class SENTINELS_LS_API ASTPlayerCharacter_GreatSword : public ASTPlayerCharacter
{
	GENERATED_BODY()

/*
	AActor Interface
*/
protected:
	virtual void BeginPlay() override;
	
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
	void SetWrapTarget_Passive();

protected:
	UPROPERTY(EditAnywhere, Category = "Skill")
	float TraceRange_Passive = 200.f;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float TraceRadius_Passive = 100.f;

private:
	UPROPERTY()
	AActor* TargetActor_Passive = nullptr;
};
