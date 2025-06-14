// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interact/InteractableActor.h"
#include "STGameplayTags.h"
#include "STDimensionDrift.generated.h"

/**
 * 
 */

UCLASS()
class SENTINELS_LS_API ASTDimensionDrift : public AInteractableActor
{
	GENERATED_BODY()

public:
	ASTDimensionDrift();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintCallable)
	FString GetLevelName(const FGameplayTag LevelTag) { return LevelMap.Find(LevelTag)->GetAssetName(); }

	//UFUNCTION(BlueprintCallable)
	//USkeletalMesh* GetSKMeshByID(int Parts, FName RowName);

protected:
	virtual void Interact_Implementation(UInteractComponent* InteractingComponent) override;
	virtual void Interact_Finish_Implementation(UInteractComponent* InteractingComponent) override;
	virtual void ShowInteractiveUI_Implementation(UInteractComponent* InteractingComponent) override;
	virtual void HideInteractiveUI_Implementation(UInteractComponent* InteractingComponent) override;

private:
	UFUNCTION()
	void HandleAllPlayerIsReady(FGameplayTag NewGameLevel);

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class USkeletalMeshComponent> SKMesh;

	/*
		Widget
	*/

	UPROPERTY(EditAnywhere, Category = "WBP")
	TSubclassOf<UUserWidget> Widget_LoadoutClass;

	UPROPERTY(EditAnywhere, Category = "WBP")
	TSubclassOf<UUserWidget> Widget_WeaponSelectClass;

	UPROPERTY(EditAnywhere, Category = "WBP")
	TSubclassOf<UUserWidget> Widget_CustomizeClass;

	UPROPERTY(EditAnywhere, Category = "WBP")
	TSubclassOf<UUserWidget> Widget_LevelSelectClass;

	/*
		Level Info
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level", meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, TSoftObjectPtr<UWorld>> LevelMap;
};
