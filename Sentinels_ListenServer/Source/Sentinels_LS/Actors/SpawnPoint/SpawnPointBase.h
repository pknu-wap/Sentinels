// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "SpawnPointBase.generated.h"

UCLASS()
class SENTINELS_LS_API ASpawnPointBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnPointBase();

public:
	bool HasTag(FGameplayTag InTag) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer TagContainer;
		
public:
	int SpawnPointIdx = 0;
};
