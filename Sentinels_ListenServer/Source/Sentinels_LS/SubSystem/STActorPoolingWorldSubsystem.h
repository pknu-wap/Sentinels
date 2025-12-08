// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Actors/ObjectPool/ActorObjectPool.h"
#include "Actors/STPoolableActor.h"
#include "STActorPoolingWorldSubsystem.generated.h"

class ASTPoolableActor;

UCLASS()
class SENTINELS_LS_API USTActorPoolingWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	USTActorPoolingWorldSubsystem();

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	UFUNCTION(BlueprintCallable)
	void InitActorPool(UObject* WorldContextObject, TSubclassOf<ASTPoolableActor> InClass, int InPoolSize);

	UFUNCTION(BlueprintCallable)
	AActor* K2_GetActor(TSubclassOf<ASTPoolableActor> InClass, FVector Location = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator);

	template<typename T>
	T* GetActor(TSubclassOf<ASTPoolableActor> InClass, FVector Location = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator)
	{
		AActorObjectPool** ObjectPool = ActorPools.Find(InClass);
		if (ObjectPool)
		{
			return (*ObjectPool)->GetActor<T>(Location, Rotation);
		}

		return nullptr;
	}

protected:
	UPROPERTY()
	TMap<TSubclassOf<ASTPoolableActor>, AActorObjectPool*> ActorPools;
};
