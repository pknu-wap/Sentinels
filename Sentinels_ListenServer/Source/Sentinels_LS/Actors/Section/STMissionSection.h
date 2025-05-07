// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
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


public:
	UFUNCTION(BlueprintImplementableEvent)
	void GetSpawnPointsWithTag(const FGameplayTag InTag, TArray<ASpawnPointBase*>& OutActors) const;

public:
	UPROPERTY(VisibleAnywhere)
	bool bIsSelected = false;

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
