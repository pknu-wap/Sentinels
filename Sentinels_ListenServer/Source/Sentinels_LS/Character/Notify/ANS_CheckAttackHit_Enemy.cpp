// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Notify/ANS_CheckAttackHit_Enemy.h"
#include "Player/STPlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Perception/AISense_Damage.h"
#include "Character/Enemy/STEnemyBase.h"

void UANS_CheckAttackHit_Enemy::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}

void UANS_CheckAttackHit_Enemy::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

    TArray<FHitResult> hitResults;
    FVector Start = MeshComp->GetSocketLocation(StartSocket);
    FVector End = MeshComp->GetSocketLocation(EndSocket);

    TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
    objectType.Emplace(UEngineTypes::ConvertToObjectType(ECC_Pawn));
    objectType.Emplace(UEngineTypes::ConvertToObjectType(ECC_Destructible));

    TArray<AActor*> ignore;
    ignore.Emplace(MeshComp->GetOwner());

    UKismetSystemLibrary::SphereTraceMultiForObjects(MeshComp, Start, End, Radius, objectType,
        false, ignore, DebugType.GetValue(), hitResults, true);

    for (FHitResult hit : hitResults)
    {
        AActor* DamagedActor = hit.GetActor();

        if (DamagedActor && !DamagedActors.Contains(DamagedActor))
        {
            DamagedActors.Add(DamagedActor);

            AActor* actor = MeshComp->GetOwner();
            if (actor && !DamagedActor->IsA(ASTEnemyBase::StaticClass()))
            {
                UGameplayStatics::ApplyPointDamage(DamagedActor, Damage, hit.ImpactNormal, hit,
                    actor->GetInstigatorController(), actor, UDamageType::StaticClass());
            }

            UAISense_Damage::ReportDamageEvent(DamagedActor, DamagedActor, actor,
                Damage, DamagedActor->GetActorLocation(), hit.ImpactPoint);
        }
    }
}

void UANS_CheckAttackHit_Enemy::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    DamagedActors.Empty();
}