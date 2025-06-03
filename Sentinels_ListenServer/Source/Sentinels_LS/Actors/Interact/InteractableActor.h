// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractiveInterface.h"
#include "Interfaces/MissionInterface.h"
#include "InteractableActor.generated.h"

class UUserWidget;
class UInteractComponent;

UCLASS()
class SENTINELS_LS_API AInteractableActor : public AActor, public IInteractiveInterface, public IMissionInterface
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
	virtual void Interact_Implementation(UInteractComponent* InteractingComponent) override;
	virtual void Interact_Finish_Implementation(UInteractComponent* InteractingComponent) override;
	virtual void ShowInteractiveUI_Implementation(UInteractComponent* InteractingComponent) override;
	virtual void HideInteractiveUI_Implementation(UInteractComponent* InteractingComponent) override;

	virtual bool IsInteractable_Implementation() override { return bIsInteractable; };

	//virtual bool IsSuccessed_Implementation() override;

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
