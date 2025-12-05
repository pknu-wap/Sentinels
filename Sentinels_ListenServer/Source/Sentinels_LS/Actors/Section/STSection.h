// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SpawnEnemyComponent.h"
#include "STSection.generated.h"

UCLASS()
class SENTINELS_LS_API ASTSection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTSection();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


/*
	Flow Logic
*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<class APlayerStart*> PlayerStarts;

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

	UFUNCTION(BlueprintCallable)
	void DespawnAllEnemys();

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
};
