// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/STEnemyBase_AIController.h"
#include "STEnemy_EntireMap_AIController.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API ASTEnemy_EntireMap_AIController : public ASTEnemyBase_AIController
{
	GENERATED_BODY()
	
public:
	virtual void OnTargetDetected(AActor* actor, const FAIStimulus Stimulus);
};
