// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Helix/STHelixAnimInstance.h"
#include "Character/Enemy/Helix/STHelix.h"

void USTHelixAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	helix = Cast<ASTHelix>(TryGetPawnOwner());
	if (!helix)
	{
		UE_LOG(LogTemp, Warning, TEXT("USTHelixAnimInstance : Failed to get Helix!"));
	}
}

void USTHelixAnimInstance::AnimNotify_Helix_Special_01()
{
	if (helix && helix->HasAuthority())
	{
		helix->StartSpawnLazers();
	}
}

void USTHelixAnimInstance::AnimNotify_Helix_Special_02()
{
	if (helix && helix->HasAuthority())
	{
		helix->StartSpawnProjectiles();
	}
}

void USTHelixAnimInstance::AnimNotify_Helix_Special_03()
{
	if (helix && helix->HasAuthority())
	{
		helix->SpawnShockWave();
	}
}

void USTHelixAnimInstance::AnimNotify_Helix_Cast()
{
	if (helix && helix->HasAuthority())
	{
		helix->StartSpawnExplosions();
	}
}
