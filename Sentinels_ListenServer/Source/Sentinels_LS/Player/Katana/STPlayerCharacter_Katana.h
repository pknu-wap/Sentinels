// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/STPlayerCharacter.h"
#include "STPlayerCharacter_Katana.generated.h"

class AProjectileBase;

UCLASS()
class SENTINELS_LS_API ASTPlayerCharacter_Katana : public ASTPlayerCharacter
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void SetWrapTarget_Q();

	UFUNCTION()
	void SpawnSlash_Q();

protected:
	UPROPERTY(EditAnywhere, Category = "Skill")
	float WarpDistance_Q = 500.f;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AActor> SubclassOfSlash_Q;

private:
	FVector SpawnLocation_Slash_Q;
};
