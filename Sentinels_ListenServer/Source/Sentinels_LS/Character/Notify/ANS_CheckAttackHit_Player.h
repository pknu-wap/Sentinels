// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "STEnums.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DamageType/STDamageTypes.h"
#include "ANS_CheckAttackHit_Player.generated.h"

class ASTPlayerCharacter;
class USTPlayerStatusComponent;
class UDamageType;
class USTBaseDamageType;
struct FSTPointDamageEvent;

UCLASS()
class SENTINELS_LS_API UANS_CheckAttackHit_Player : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

private:
	void CalculateFinalDamage();
	TSubclassOf<UDamageType> GetDamageType() const;

public:
	UPROPERTY(EditAnywhere, Category = Damage)
	FName StartSocket;

	UPROPERTY(EditAnywhere, Category = Damage)
	FName EndSocket;

	UPROPERTY(EditAnywhere, Category = Damage)
	float Radius = 30.f;

	UPROPERTY(EditAnywhere, Category = Damage)
	TEnumAsByte<EDrawDebugTrace::Type> DebugType = EDrawDebugTrace::None;

	UPROPERTY(EditAnywhere, Category = Damage)
	TSubclassOf<USTBaseDamageType> DamageType;

	UPROPERTY(EditAnywhere, Category = Damage)
	float DamagePercent = 1.f;

private:
	TArray<AActor*> DamagedActors;

	UPROPERTY()
	ASTPlayerCharacter* Player;

	UPROPERTY()
	USTPlayerStatusComponent* StatusComp;

	UPROPERTY()
	float FinalDamage = 10.f;

	UPROPERTY()
	FSTPointDamageEvent DamageEvent;
};
