// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "STEnums.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AN_ApplyRadialDamage.generated.h"

class ASTPlayerCharacter;
class USTBaseDamageType;

UCLASS()
class SENTINELS_LS_API UAN_ApplyRadialDamage : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	void CalculateFinalDamage();
	TSubclassOf<UDamageType> GetDamageType() const;

protected:
	UPROPERTY(EditAnywhere, Category = Damage)
	TEnumAsByte<EDrawDebugTrace::Type> DebugType = EDrawDebugTrace::None;

	UPROPERTY(EditAnywhere, Category = Damage)
	float ForwardOffset = 0.f;

	UPROPERTY(EditAnywhere, Category = Damage)
	float Radius = 300.f;

	UPROPERTY(EditAnywhere, Category = Damage)
	TSubclassOf<USTBaseDamageType> DamageType;

	UPROPERTY(EditAnywhere, Category = Damage)
	float DamagePercent = 1.f;

private:
	UPROPERTY()
	TArray<AActor*> DamagedActors;

	UPROPERTY()
	float FinalDamage = 10.f;
};
