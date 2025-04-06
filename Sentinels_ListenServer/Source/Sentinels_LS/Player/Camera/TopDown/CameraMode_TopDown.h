// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Camera/CameraMode.h"
#include "CameraMode_TopDown.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class SENTINELS_LS_API UCameraMode_TopDown : public UCameraMode
{
    GENERATED_BODY()

public:
    virtual void OnEnter(class UCameraModeManagerComponent* Manager) override;
    virtual void OnExit() override;
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY()
    USpringArmComponent* SpringArm;

    UPROPERTY()
    UCameraComponent* Camera;

    bool bShouldLerp = false;

};
