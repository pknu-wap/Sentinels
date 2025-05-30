// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Notify/ANS_CheckAttackHit_Player.h"
#include "Player/STPlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Perception/AISense_Damage.h"
#include "Components/STPlayerStatusComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"

void UANS_CheckAttackHit_Player::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration);

    bTimeDilationApplied = false;
    Player = Cast<ASTPlayerCharacter>(MeshComp->GetOwner());
    CalculateFinalDamage();

    Value_TimeDilation = 0.01f;
    Duration_TimeDilation = 0.025f;
}

void UANS_CheckAttackHit_Player::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

    Player = Cast<ASTPlayerCharacter>(MeshComp->GetOwner());

    if (Player)
    {
        if(MeshComp != Player->GetMesh() || !Player->HasAuthority())
            return;
    }

    if (!StatusComp) return;

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
        float damage = 0.f;
        AActor* DamagedActor = hit.GetActor();

        if (DamagedActor && !DamagedActors.Contains(DamagedActor))
        {
            DamagedActors.Add(DamagedActor);

            AActor* actor = MeshComp->GetOwner();
            if (actor && !DamagedActor->IsA(ASTPlayerCharacter::StaticClass()))
            {
                // Check Critical
                bool bIsCritical = UKismetMathLibrary::RandomFloatInRange(0, 1) <= StatusComp->CriticalRate ? true : false;
                damage = bIsCritical ? CiriticalFinalDamage : FinalDamage;

                // Generate Damage Event With Critical
                DamageEvent = FSTPointDamageEvent(bIsCritical, damage, hit, hit.ImpactNormal, GetDamageType());

                // Player Logic Execute (Pre)
                Player->AdjustFinalDamage(damage, DamageEvent, DamagedActor);

                // Apply Damage
                DamagedActor->TakeDamage(damage, DamageEvent, actor->GetInstigatorController(), actor);
               
                // Player Logic Execute (After)
                Player->OnAttackSuccess_Server(damage, DamageEvent, DamagedActor);



                // Apply Time Dilation
                if (!bTimeDilationApplied)
                {
                    ASTPlayerCharacter* player = Cast<ASTPlayerCharacter>(actor);
                    if (player)
                    {
                        player->ApplyCustomTimeDilation(Value_TimeDilation, Duration_TimeDilation);
                        player->ApplyAttackCameraShake();
                        bTimeDilationApplied = true;
                    }
                }

                // Spawn Impact Particle Emmiter
                UGameplayStatics::SpawnEmitterAtLocation(DamagedActor->GetWorld(), ImpactParticle, hit.ImpactPoint);
               
            }

            UAISense_Damage::ReportDamageEvent(DamagedActor, DamagedActor, actor,
                damage, DamagedActor->GetActorLocation(), hit.ImpactPoint);
        }
    }
}

void UANS_CheckAttackHit_Player::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    bTimeDilationApplied = false;
    DamagedActors.Empty();
}

void UANS_CheckAttackHit_Player::CalculateFinalDamage()
{
    if (Player)
    {
        AController* PC = Player->GetController();
        if (!PC) return;

        StatusComp = Player->GetComponentByClass<USTPlayerStatusComponent>();
        if (!StatusComp) return;

        // ATK  * CriticalDamagePercent * (1 + DamageIncreaseRate) * DamagePercent
        FinalDamage = StatusComp->GetBaseDamage() * DamagePercent;
        CiriticalFinalDamage = StatusComp->GetCriticalBaseDamage() * DamagePercent;

        if (FinalDamage >= 15.f)
        {
            UE_LOG(LogTemp, Display, TEXT("Failed"));
        }
    }
}


TSubclassOf<UDamageType> UANS_CheckAttackHit_Player::GetDamageType() const
{
    if (DamageType)
        return DamageType;

    return UDamageType::StaticClass();
}
