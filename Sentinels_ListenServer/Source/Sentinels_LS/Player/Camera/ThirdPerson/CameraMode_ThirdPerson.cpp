// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Camera/ThirdPerson/CameraMode_ThirdPerson.h"
#include "Components/CameraModeManagerComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/STPlayerCharacter.h"
#include "Player/STPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void UCameraMode_ThirdPerson::OnEnter(UCameraModeManagerComponent* Manager)
{
    CameraManager = Manager;

    Player = Cast<ASTPlayerCharacter>(Manager->GetOwner());
    if (Player)
    {
        SpringArm = Player->GetComponentByClass<USpringArmComponent>();
        Camera = Player->GetComponentByClass<UCameraComponent>();
    }

    // Set Input
    PC = Cast<ASTPlayerController>(Player->GetController());
    if (Player && PC)
    {
        Player->ClearAllMappingContext();
        PC->DisableInput(PC);
        bShouldLerp = true;
    }
}

void UCameraMode_ThirdPerson::OnExit()
{
}

void UCameraMode_ThirdPerson::Tick(float DeltaTime)
{
    if (bShouldLerp && Camera && SpringArm)
    {
        SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, TargetArmLength, DeltaTime, 2.5f);
        PC->SetControlRotation(FMath::RInterpTo(PC->GetControlRotation(), FRotator(0, -90, 0), DeltaTime, 2.5f));

        Camera->SetRelativeRotation(FMath::RInterpTo(Camera->GetRelativeRotation(), TargetRotation, DeltaTime, 2.5f));
        Camera->SetRelativeLocation(FMath::VInterpTo(Camera->GetRelativeLocation(), TargetOffset, DeltaTime, 2.5f));

        if (Camera->GetRelativeRotation().Equals(TargetRotation, 1.0f)
            && Camera->GetRelativeLocation().Equals(TargetOffset, 1.0f)
            && abs(SpringArm->TargetArmLength - TargetArmLength) < 2.5f)
        {
            bShouldLerp = false;

            if (Player && PC)
            {
                Player->BindDefaultThirdPersonInput();
                PC->BindDefaultThirdPersonInput();
                PC->EnableInput(PC);
            }
        }
    }
}
