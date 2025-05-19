// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/STPoolableActor.h"
#include "ActorObjectPool.generated.h"

class ASTPoolableActor;

UCLASS()
class SENTINELS_LS_API AActorObjectPool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorObjectPool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void InitPool(TSubclassOf<ASTPoolableActor> InObjectClass, int PoolSize);

	template <typename T>
	T* GetActor(FVector Location = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator)
	{
		for (ASTPoolableActor* Actor : ObjectPool)
		{
			if (Actor && !Actor->IsActivated())
			{
				Actor->Activate(Location, Rotation);
				return Cast<T>(Actor);
			}
		}

		ASTPoolableActor* actor = Cast<T>(GetWorld()->SpawnActor(ObjectClass));
		if (actor)
		{
			actor->Activate(Location, Rotation);
			ObjectPool.Add(actor);
			return  Cast<T>(actor);
		}

		return nullptr;
	}

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pool")
	TArray<ASTPoolableActor*> ObjectPool;

	UPROPERTY()
	TSubclassOf<ASTPoolableActor> ObjectClass;

};
