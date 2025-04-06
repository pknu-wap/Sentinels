// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CameraMode.generated.h"

class UCameraModeManagerComponent;

UCLASS(Abstract, Blueprintable, EditInlineNew)
class SENTINELS_LS_API UCameraMode : public UObject
{
    GENERATED_BODY()

public:
    virtual void OnEnter(class UCameraModeManagerComponent* Manager) {};
    virtual void OnExit() {};
    virtual void Tick(float DeltaTime) {};

protected:
    UPROPERTY()
    UCameraModeManagerComponent* CameraManager;

protected:
    UPROPERTY(EditAnywhere)
    float TargetArmLength = 300.f;

    UPROPERTY(EditAnywhere)
    FVector TargetOffset;

    UPROPERTY(EditAnywhere)
    FRotator TargetRotation = FRotator::ZeroRotator;

};
