// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/STPoolableCharacter.h"
#include "CharacterObjectPool.generated.h"

class ASTPoolableCharacter;

UCLASS()
class SENTINELS_LS_API ACharacterObjectPool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterObjectPool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void InitPool(TSubclassOf<ASTPoolableCharacter> InObjectClass, int PoolSize);

	template <typename T>
	T* GetCharacter(FVector Location = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator)
	{
		for (ASTPoolableCharacter* character : ObjectPool)
		{
			if (character && !character->IsActivated())
			{
				character->Activate(Location, Rotation);
				return Cast<T>(character);
			}
		}

		ASTPoolableCharacter* character = Cast<T>(UAIBlueprintHelperLibrary::SpawnAIFromClass(this, ObjectClass, nullptr,
			Location, Rotation, true));
		if (character)
		{
			character->Activate(Location, Rotation);
			ObjectPool.Add(character);
			return  Cast<T>(character);
		}

		return nullptr;
	}

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pool")
	TArray<ASTPoolableCharacter*> ObjectPool;

	UPROPERTY()
	TSubclassOf<ASTPoolableCharacter> ObjectClass;
};
