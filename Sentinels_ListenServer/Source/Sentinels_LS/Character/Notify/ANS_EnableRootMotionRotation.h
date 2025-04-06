// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_EnableRootMotionRotation.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API UANS_EnableRootMotionRotation : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere)
	FRotator NewRotationRate = FRotator(0.f, 500.f, 0.f);

private:
	UPROPERTY()
	ACharacter* Character;

	FRotator DefaultRotationRate;
};
