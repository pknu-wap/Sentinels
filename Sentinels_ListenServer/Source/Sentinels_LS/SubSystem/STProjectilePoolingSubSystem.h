// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Actors/ObjectPool/ActorObjectPool.h"
#include "STProjectilePoolingSubSystem.generated.h"

class AProjectileBase;
class AActorObjectPool;

UCLASS()
class SENTINELS_LS_API USTProjectilePoolingSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	USTProjectilePoolingSubSystem();

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	void InitProjectilePool(UObject* WorldContextObject, TSubclassOf<AProjectileBase> InClass, int InPoolSize);
	
	template<typename T>
	T* GetActor(TSubclassOf<AProjectileBase> InClass, FVector Location = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator)
	{
		AActorObjectPool** ObjectPool = ProjectilePools.Find(InClass);
		if (ObjectPool)
		{
			return (*ObjectPool)->GetActor<T>(Location, Rotation);
		}

		return nullptr;
	}

protected:
	UPROPERTY()
	TMap<TSubclassOf<AProjectileBase>, AActorObjectPool*> ProjectilePools;
};
