// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AnimNotify_ApplyRadialDamage.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API UAnimNotify_ApplyRadialDamage : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:

	UPROPERTY(EditAnywhere, Category = Damage)
	FName SocketName;

	UPROPERTY(EditAnywhere, Category = Damage)
	FVector LocationOffset;

	UPROPERTY(EditAnywhere, Category = Damage)
	float Radius = 30.f;

	UPROPERTY(EditAnywhere, Category = Damage)
	TSubclassOf<AActor> DamagedActorClass;

	UPROPERTY(EditAnywhere, Category = Damage)
	float BaseDamage = 10.f;

	UPROPERTY(EditAnywhere, Category = Damage)
	TEnumAsByte<EDrawDebugTrace::Type> DebugType = EDrawDebugTrace::None;


};
