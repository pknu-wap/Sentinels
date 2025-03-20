// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractiveInterface.h"
#include "InteractableActor.generated.h"

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

	/*
		Interactive Interface
	*/
protected:
	virtual void Interact() override;
	virtual void ShowInteractiveUI() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UStaticMeshComponent> Mesh;
};
