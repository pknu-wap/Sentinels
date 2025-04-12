// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_LaunchSlash.generated.h"

class AProjectileBase;

UCLASS()
class SENTINELS_LS_API UAN_LaunchSlash : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AProjectileBase> SubclassOfProjectileSlash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SocketName;

	/*
		Roll Offset for Slash
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RollOffset = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator SlashRotationOffset = FRotator::ZeroRotator;

	/*
		Apply Rotation Offset When Launch
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator LaunchRotationOffset = FRotator::ZeroRotator;
};
