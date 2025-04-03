// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_ConvertMovementMode.generated.h"

class ACharacter;

UCLASS()
class SENTINELS_LS_API UANS_ConvertMovementMode : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EMovementMode> InMovementMode;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EMovementMode> OutMovementMode;

private:
	UPROPERTY()
	ACharacter* Character;
};
