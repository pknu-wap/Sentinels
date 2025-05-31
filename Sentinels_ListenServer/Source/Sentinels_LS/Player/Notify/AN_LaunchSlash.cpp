// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Notify/AN_LaunchSlash.h"
#include "Character/STCharacterBase.h"
#include "Actors/Projectile/ProjectileBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "SubSystem/STProjectilePoolingSubSystem.h"

void UAN_LaunchSlash::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ASTCharacterBase* OwningCharacter = Cast<ASTCharacterBase>(MeshComp->GetOwner());
	if (OwningCharacter && OwningCharacter->HasAuthority())
	{
		USTProjectilePoolingSubSystem* PoolingSystem = OwningCharacter->GetWorld()->GetSubsystem<USTProjectilePoolingSubSystem>();
		if (!PoolingSystem) return;

		FVector Location_Katana = MeshComp->GetSocketLocation(SocketName);
		FRotator Rotation_Slash = FRotator(0, 0, RollOffset + UKismetMathLibrary::RandomFloatInRange(-10, 10));
		Rotation_Slash = SlashRotationOffset + FRotator(0, 0, UKismetMathLibrary::RandomFloatInRange(-10, 10));

		// AProjectileBase* Slash = OwningCharacter->GetWorld()->SpawnActor<AProjectileBase>(SubclassOfProjectileSlash, Location_Katana, Rotation_Slash + OwningCharacter->GetActorRotation());
		AProjectileBase* Slash = PoolingSystem->GetActor<AProjectileBase>(SubclassOfProjectileSlash, Location_Katana, Rotation_Slash + OwningCharacter->GetActorRotation());
		if (!Slash)
		{
			PoolingSystem->InitProjectilePool(OwningCharacter, SubclassOfProjectileSlash, 20);
			Slash = PoolingSystem->GetActor<AProjectileBase>(SubclassOfProjectileSlash, Location_Katana, Rotation_Slash + OwningCharacter->GetActorRotation());
		}

		if (Slash)
		{
			Slash->SetInstigator(Cast<APawn>(MeshComp->GetOwner()));
			if (LaunchRotationOffset == FRotator::ZeroRotator)
			{
				Slash->FireInDirection(OwningCharacter->GetActorForwardVector());
			}
			else
			{
				Slash->FireInDirection((OwningCharacter->GetActorForwardVector().Rotation() + LaunchRotationOffset).Vector());
			}
		}
	}
}
