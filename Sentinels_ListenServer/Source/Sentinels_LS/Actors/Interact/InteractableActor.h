// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractiveInterface.h"
#include "InteractableActor.generated.h"

class UUserWidget;
class UInteractComponent;

UCLASS()
class SENTINELS_LS_API AInteractableActor : public AActor, public IInteractiveInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

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
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = "Debug")
	TSubclassOf<UUserWidget> InteractWidgetClass_ForDebug;

	UPROPERTY()
	UUserWidget* InteractWidget_ForDebug;

	UPROPERTY(Replicated, VisibleAnywhere)
	bool bIsInteractable = true;
};
