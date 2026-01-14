// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Notify/AnimNotify_ApplyRadialDamage.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Damage.h"

void UAnimNotify_ApplyRadialDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp)
	{
        /*
            Calculate Transform
        */
		FTransform transform = MeshComp->GetComponentTransform();
		if (SocketName.IsNone())
		{
			transform = MeshComp->GetComponentTransform();
		}
		else
		{
			transform = MeshComp->GetSocketTransform(SocketName);
		}

        transform.SetLocation(transform.TransformPosition(LocationOffset));

        /*
            Tracing
        */
        TArray<FHitResult> hitResults;

        TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
        objectType.Emplace(UEngineTypes::ConvertToObjectType(ECC_Pawn));

        TArray<AActor*> ignore;
        ignore.Emplace(MeshComp->GetOwner());

        UKismetSystemLibrary::SphereTraceMultiForObjects(MeshComp, transform.GetLocation(), transform.GetLocation(), Radius, objectType,
            false, ignore, DebugType.GetValue(), hitResults, true, FLinearColor::Red, FLinearColor::Green, 3.f);


        /*
            Apply Damage
        */
        TArray<AActor*> DamagedActors;
        for (FHitResult hit : hitResults)
        {
            AActor* DamagedActor = hit.GetActor();

            if (DamagedActor && !DamagedActors.Contains(DamagedActor))
            {
                DamagedActors.Add(DamagedActor);

                AActor* actor = MeshComp->GetOwner();
                if (actor && DamagedActor->IsA(DamagedActorClass))
                {
                    UGameplayStatics::ApplyPointDamage(DamagedActor, BaseDamage, hit.ImpactNormal, hit,
                        actor->GetInstigatorController(), actor, UDamageType::StaticClass());
                }

                UAISense_Damage::ReportDamageEvent(DamagedActor, DamagedActor, actor,
                    BaseDamage, DamagedActor->GetActorLocation(), hit.ImpactPoint);
            }
        }
	}
}
