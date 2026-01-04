// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/STEliteBase.h"
#include "STSevarog.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API ASTSevarog : public ASTEliteBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void UltimateFireCallback();

protected:
	void StartSoulSiphon();
	void ApplySoulSiphonDamage();

	void SpawnSubjection();
	void SpawnAOE();

protected:
	UPROPERTY()
	class USTCharacterAnimInstanceBase* AnimInst;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SubjectionActorClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> AOEActorClass;

protected:
	UPROPERTY()
	AActor* SoulSiphonTarget;

	int SoulSiphonCount;

	FTimerHandle SoulSiphonHandle;
};
