// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraModeManagerComponent.generated.h"

class UCameraMode;
class USpringArmComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENTINELS_LS_API UCameraModeManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraModeManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void InitCameraMode();

	UFUNCTION(BlueprintCallable)
    void SetCameraMode(TSubclassOf<UCameraMode> NewModeClass);

	USpringArmComponent* GetSpringArm() const { return SpringArm; }

private:
    UPROPERTY()
    UCameraMode* CurrentMode;

	UPROPERTY()
    USpringArmComponent* SpringArm;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UCameraMode> DefaultCameraMode;
};
