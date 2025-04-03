// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Camera/TopDown/CameraMode_TopDown.h"
#include "Components/CameraModeManagerComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/STPlayerCharacter.h"
#include "Player/STPlayerController.h"

void UCameraMode_TopDown::OnEnter(UCameraModeManagerComponent* Manager)
{
    CameraManager = Manager;

    ASTPlayerCharacter* Player = Cast<ASTPlayerCharacter>(Manager->GetOwner());
    if (Player)
    {
        SpringArm = Player->GetComponentByClass<USpringArmComponent>();
        Camera = Player->GetComponentByClass<UCameraComponent>();
    }

    ASTPlayerController* PC = Cast<ASTPlayerController>(Player->GetController());
    if (Player && PC)
    {
        Player->BindDefaultTopDownInput();
        PC->BindDefaultTopDownInput();
        PC->DisableInput(PC);
        bShouldLerp = true;
    }
}

void UCameraMode_TopDown::OnExit()
{
}

void UCameraMode_TopDown::Tick(float DeltaTime)
{
    if (bShouldLerp && Camera && SpringArm)
    {
        SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, TargetArmLength, DeltaTime, 2.5f);
        Camera->SetWorldRotation(FMath::RInterpTo(Camera->GetComponentRotation(), TargetRotation, DeltaTime, 2.5f));
        // SpringArm->TargetOffset = FMath::VInterpTo(SpringArm->TargetOffset, TargetOffset, DeltaTime, 2.5f);
        Camera->SetRelativeLocation(FMath::VInterpTo(Camera->GetRelativeLocation(), TargetOffset, DeltaTime, 2.5f));

        if (Camera->GetComponentRotation().Equals(TargetRotation, 1.0f)
            // && SpringArm->TargetOffset.Equals(TargetOffset, 1.0f)
            && Camera->GetRelativeLocation().Equals(TargetOffset, 1.0f)
            && abs(SpringArm->TargetArmLength - TargetArmLength) < 2.5f)
        {
            bShouldLerp = false;

            APawn* Player = Cast<APawn>(Camera->GetOwner());
            if (!Player) return;

            APlayerController* PC = Cast<APlayerController>(Player->GetController());
            if (PC)
            {
                PC->EnableInput(PC);
            }
        }
    }
}
