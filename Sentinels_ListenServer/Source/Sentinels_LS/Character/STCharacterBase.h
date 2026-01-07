// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "DamageType/STDamageTypes.h"
#include "STStructs.h"
#include "STCharacterBase.generated.h"

/*
	- Character Base Class.
	- All Characters should be Subclass of this.
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateAdd, FGameplayTag, AddedState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateRemove, FGameplayTag, RemovedState);

UCLASS()
class SENTINELS_LS_API ASTCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintCallable)
	void ApplyCustomDamage(float Damage, struct FSTPointDamageEvent DamageEvent, TSubclassOf<UDamageType> DamageType, AController* EventInstigator, AActor* DamageCauser);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void Jump() override;

	UFUNCTION()
	void OnCharacterLanded(const FHitResult& Hit);


public:
	UFUNCTION(BlueprintCallable)
	void AddUniqueTag(const FGameplayTag& TagToAdd) 
	{
		TagContainer.AddUnique(FStatusEntry(TagToAdd, 1));
	}

	UFUNCTION(BlueprintCallable)
	void AddTag(const FGameplayTag& TagToAdd, int NumToAdd = 1) 
	{ 
		for (FStatusEntry& entry : TagContainer)
		{
			if (entry.StatusTag == TagToAdd)
			{
				entry.Count += NumToAdd;
				return;
			}
		}

		TagContainer.Add(FStatusEntry(TagToAdd, NumToAdd));
	}

	UFUNCTION(BlueprintCallable)
	void ClearTag(const FGameplayTag& TagToRemove) 
	{ 
		for (FStatusEntry& entry : TagContainer)
		{
			if (entry.StatusTag == TagToRemove)
			{
				TagContainer.RemoveSingle(entry);
				return;
			}
		}
	}

	UFUNCTION(BlueprintCallable)
	void RemoveTag(const FGameplayTag& TagToRemove, int NumToRemove)
	{
		for (FStatusEntry& entry : TagContainer)
		{
			if (entry.StatusTag == TagToRemove)
			{
				entry.Count -= NumToRemove;

				if (entry.Count <= 0)
				{
					TagContainer.RemoveSingle(entry);
				}

				return;
			}
		}
	}

	UFUNCTION(BlueprintCallable)
	void RemoveTags(const FGameplayTagContainer& TagsToRemove) 
	{
		TagContainer.Empty(); 
	}

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	bool HasTag(const FGameplayTag& TagToCheck) const 
	{ 
		for (const FStatusEntry& entry : TagContainer)
		{
			if (entry.StatusTag == TagToCheck)
			{
				return true;
			}
		}

		return false; 
	}

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	int GetNumOfTag(const FGameplayTag& TagToCheck) const
	{
		for (const FStatusEntry& entry : TagContainer)
		{
			if (entry.StatusTag == TagToCheck)
			{
				return entry.Count;
			}
		}

		return -1;
	}

	UFUNCTION(NetMulticast, Reliable)
	void UpdateEnemyStateWidget_Multicast(FGameplayTag StateTag, bool bShow);

public:
	UPROPERTY(BlueprintAssignable)
	FOnStateAdd Delegate_OnStateAdd;

	UPROPERTY(BlueprintAssignable)
	FOnStateRemove Delegate_OnStateRemove;

protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	TArray<FStatusEntry> TagContainer;

};
