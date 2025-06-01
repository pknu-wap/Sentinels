// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "DamageType/STDamageTypes.h"
#include "STCharacterBase.generated.h"

/*
	- Character Base Class.
	- All Characters should be Subclass of this.
*/

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
	void AddTag(const FGameplayTag& TagToAdd) { TagContainer.AddTag(TagToAdd); }
	UFUNCTION(BlueprintCallable)
	void RemoveTag(const FGameplayTag& TagToRemove) { TagContainer.RemoveTag(TagToRemove); }
	UFUNCTION(BlueprintCallable)
	void RemoveTags(const FGameplayTagContainer& TagsToRemove) { TagContainer.RemoveTags(TagsToRemove); }
	UFUNCTION(BlueprintCallable)
	bool HasTag(const FGameplayTag& TagToCheck) const { return TagContainer.HasTag(TagToCheck); }

protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	FGameplayTagContainer TagContainer;
};
