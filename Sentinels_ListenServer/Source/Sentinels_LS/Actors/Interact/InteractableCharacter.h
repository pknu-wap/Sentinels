// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/STCharacterBase.h"
#include "Interfaces/InteractiveInterface.h"
#include "InteractableCharacter.generated.h"

class UUserWidget;
class UInteractComponent;

UCLASS()
class SENTINELS_LS_API AInteractableCharacter : public ASTCharacterBase, public IInteractiveInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AInteractableCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
		Interactive Interface
	*/
protected:
	virtual void Interact(UInteractComponent* InteractingComponent) override;
	virtual void Interact_Finish(UInteractComponent* InteractingComponent) override;
	virtual void ShowInteractiveUI(UInteractComponent* InteractingComponent) override;
	virtual void HideInteractiveUI(UInteractComponent* InteractingComponent) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Debug")
	TSubclassOf<UUserWidget> InteractWidgetClass_ForDebug;

	UPROPERTY()
	UUserWidget* InteractWidget_ForDebug;
};
