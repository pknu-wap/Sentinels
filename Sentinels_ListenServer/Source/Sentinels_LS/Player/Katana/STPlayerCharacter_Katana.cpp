// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Katana/STPlayerCharacter_Katana.h"
#include "Player/STPlayerAnimInstance.h"
#include "Actors/Projectile/ProjectileBase.h"
#include "MotionWarpingComponent.h"

void ASTPlayerCharacter_Katana::BeginPlay()
{
	Super::BeginPlay();

	USTPlayerAnimInstance* AnimInst = Cast<USTPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInst)
	{
		AnimInst->Delegate_SetWarpTarget_Q.AddUObject(this, &ASTPlayerCharacter_Katana::SetWrapTarget_Q);
		AnimInst->Delegate_SpawnSlash_Q.AddUObject(this, &ASTPlayerCharacter_Katana::SpawnSlash_Q);
	}
}

void ASTPlayerCharacter_Katana::SetWrapTarget_Q()
{
	UMotionWarpingComponent* motionWarpComp = GetComponentByClass<UMotionWarpingComponent>();
	if (motionWarpComp)
	{
		FMotionWarpingTarget WarpTarget;
		WarpTarget.Name = FName("WarpTarget");
		WarpTarget.Location = GetActorLocation() + GetControlRotation().Vector() * WarpDistance_Q;
		WarpTarget.Rotation = GetControlRotation();

		SpawnLocation_Slash_Q = GetActorLocation() + GetControlRotation().Vector() * WarpDistance_Q * 0.5f;

		motionWarpComp->AddOrUpdateWarpTarget(WarpTarget);
	}
}

void ASTPlayerCharacter_Katana::SpawnSlash_Q()
{
	if (HasAuthority())
	{
		GetWorld()->SpawnActor<AActor>(SubclassOfSlash_Q, SpawnLocation_Slash_Q, FRotator::ZeroRotator);
	}
}
