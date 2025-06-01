// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Notify/AN_ApplyRadialDamage.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameState.h"
#include "Perception/AISense_Damage.h"
#include "DamageType/STDamageTypes.h"
#include "Components/STPlayerStatusComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DamageEvents.h"

void UAN_ApplyRadialDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation);

	AActor* Owner = MeshComp->GetOwner();
	if (Owner && Owner->HasAuthority())
	{
		CalculateFinalDamage(MeshComp, Animation, EventReference);

		TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
		objectType.Emplace(UEngineTypes::ConvertToObjectType(ECC_Pawn));
		objectType.Emplace(UEngineTypes::ConvertToObjectType(ECC_Destructible));

		TArray<AActor*> ignore;
		int playerNum = UGameplayStatics::GetNumPlayerControllers(Owner);
		for (int i = 0; i < playerNum; i++)
		{
			APlayerController* controller = UGameplayStatics::GetPlayerController(Owner, i);
			if (controller)
			{
				ignore.Add(controller->GetPawn());
			}
		}

		FVector TraceStartLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * ForwardOffset;

		TArray<FHitResult> hitResults;
		UKismetSystemLibrary::SphereTraceMultiForObjects(Owner, TraceStartLocation, TraceStartLocation,
			Radius, objectType, false, ignore, DebugType.GetValue(), hitResults, true);

		for (FHitResult hit : hitResults)
		{
			float damage = 0.f;
			AActor* DamagedActor = hit.GetActor();
			AActor* DamageCauser = MeshComp->GetOwner();

			if (DamageCauser && DamagedActor && !DamagedActors.Contains(DamagedActor))
			{
				DamagedActors.Add(DamagedActor);

				FSTPointDamageEvent DamageEvent;

				FSTDamageInfo DamageInfo = StatusComp->GetCalculatedDamageInfo(DamageEvent, DamagedActor);

				damage = DamageInfo.DamageAmount;
				DamageEvent.bIsCritical = DamageInfo.bIsCritical;
				DamageEvent.DamageTypeClass = GetDamageType();

				// Apply Damage
				DamagedActor->TakeDamage(damage, DamageEvent, Owner->GetInstigatorController(), Owner);

				/*UGameplayStatics::ApplyPointDamage(DamagedActor, FinalDamage, hit.ImpactNormal, hit,
					DamageCauser->GetInstigatorController(), DamageCauser, GetDamageType());*/

				UAISense_Damage::ReportDamageEvent(DamagedActor, DamagedActor, DamageCauser,
					damage, DamagedActor->GetActorLocation(), hit.ImpactPoint);
			}
		}

		DamagedActors.Empty();
	}
}

void UAN_ApplyRadialDamage::CalculateFinalDamage(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	/*if (AActor* Owner = MeshComp->GetOwner())
	{
		StatusComp = Owner->GetComponentByClass<USTPlayerStatusComponent>();
		if (!StatusComp) return;

		FinalDamage = StatusComp->GetBaseDamage() * DamagePercent;
		CiriticalFinalDamage = StatusComp->GetCriticalBaseDamage() * DamagePercent;
		return;
	}
	
	FinalDamage = 10 * DamagePercent;*/
}

TSubclassOf<UDamageType> UAN_ApplyRadialDamage::GetDamageType() const
{
	if (DamageType)
		return DamageType;

	return UDamageType::StaticClass();
}
