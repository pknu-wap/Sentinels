// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/InteractiveInterface.h"
#include "InteractablePawn.generated.h"

UCLASS()
class SENTINELS_LS_API AInteractablePawn : public APawn, public IInteractiveInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AInteractablePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	/*
	Interactive Interface
	*/
protected:
	virtual void Interact(UInteractComponent* InteractingComponent) override;
	virtual void Interact_Finish(UInteractComponent* InteractingComponent) override;
	virtual void ShowInteractiveUI(UInteractComponent* InteractingComponent) override;
	virtual void HideInteractiveUI(UInteractComponent* InteractingComponent) override;

	virtual bool IsInteractable() override { return bIsInteractable; };

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = "Debug")
	TSubclassOf<UUserWidget> InteractWidgetClass_ForDebug;

	UPROPERTY()
	UUserWidget* InteractWidget_ForDebug;

	UPROPERTY(Replicated, VisibleAnywhere)
	bool bIsInteractable = true;
};
