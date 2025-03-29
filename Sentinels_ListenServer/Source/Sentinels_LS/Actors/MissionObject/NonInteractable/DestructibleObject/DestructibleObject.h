// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MissionObject/MissionObject.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "DestructibleObject.generated.h"

UCLASS()
class SENTINELS_LS_API ADestructibleObject : public AMissionObject
{
	GENERATED_BODY()

public:
	ADestructibleObject();

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION()
	void OnGCBreaked(const FChaosBreakEvent& BreakEvent);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterialInterface* MI_Outline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UGeometryCollectionComponent> GCComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth = 100.f;

	float CurrentHealth;

private:
	FTimerHandle Handle_Outline;

};
