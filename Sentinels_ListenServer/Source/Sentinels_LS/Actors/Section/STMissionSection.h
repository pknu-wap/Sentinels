// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "STStructs.h"
#include "Components/SpawnEnemyComponent.h"
#include "STMissionSection.generated.h"

class ASpawnPointBase;

UCLASS()
class SENTINELS_LS_API ASTMissionSection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTMissionSection();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

/*
	Mission
*/
public:
	UFUNCTION(BlueprintImplementableEvent)
	void GetSpawnPointsWithTag(const FGameplayTag InTag, TArray<ASpawnPointBase*>& OutActors) const;

	UFUNCTION(BlueprintCallable)
	void RegisterRandomMission();

	UFUNCTION(BlueprintImplementableEvent)
	void MissionActivated();

	UFUNCTION(BlueprintImplementableEvent)
	void MissionDeactivated();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<class APlayerStart*> PlayerStarts;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	TArray<FRegisterMissionInfo> MissionInfos;

	UPROPERTY(VisibleAnywhere)
	bool bIsSelected = false;

/*
	Spawn Enemy
*/
public:
	UFUNCTION(BlueprintCallable)
	void StartSpawnEnemy();

	UFUNCTION(BlueprintCallable)
	void StopSpawnEnemy();

protected:
	void TrySpawnAI();
	void SpawnEnemy(int InfoIdx);
	bool IsInFrontalCone(const FVector& locationToCheck, const FVector& originLocation, const FVector& forwardVector, float angleDeg) const;
	bool GetSpawnNavLocationForPlayer(int playerIdx, int infoIdx, FNavLocation& OutLocation) const;

protected:
	UFUNCTION()
	void OnEnemyDied(AActor* DiedEnemy);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	bool bShouldLoop = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TArray<FSpawnInfo> SpawnInfos;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> Players;

private:
	TQueue<int> SpawnReserveQue;

	UPROPERTY()
	TSet<int> SpawnReserveSet;

	UPROPERTY()
	FTimerHandle SpawnHandle;

	float CurrentSpawned = 0;


public:
	UFUNCTION(BlueprintCallable)
	void AddTag(const FGameplayTag& TagToAdd) { TagContainer.AddTag(TagToAdd); }
	UFUNCTION(BlueprintCallable)
	void RemoveTag(const FGameplayTag& TagToRemove) { TagContainer.RemoveTag(TagToRemove); }
	UFUNCTION(BlueprintCallable)
	void RemoveTags(const FGameplayTagContainer& TagsToRemove) { TagContainer.RemoveTags(TagsToRemove); }
	UFUNCTION(BlueprintCallable)
	bool HasTag(const FGameplayTag& TagToCheck) const { return TagContainer.HasTag(TagToCheck); }

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer TagContainer;
};
