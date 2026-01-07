// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/STCharacterBase.h"
#include "Net/UnrealNetwork.h"
#include "STGameplayTags.h"

// Sets default values
ASTCharacterBase::ASTCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASTCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	LandedDelegate.AddDynamic(this, &ASTCharacterBase::OnCharacterLanded);
}

void ASTCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASTCharacterBase, TagContainer);
}

void ASTCharacterBase::ApplyCustomDamage(float Damage, FSTPointDamageEvent DamageEvent, TSubclassOf<UDamageType> DamageType, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageType)
		DamageEvent.DamageTypeClass = DamageType;
	TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

// Called every frame
void ASTCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASTCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASTCharacterBase::Jump()
{
	if (HasTag(FSTGameplayTags::Get().Character_State_Jump)) return;

	Super::Jump();

	AddUniqueTag(FSTGameplayTags::Get().Character_State_Jump);
}

void ASTCharacterBase::OnCharacterLanded(const FHitResult& Hit)
{
	ClearTag(FSTGameplayTags::Get().Character_State_Jump);
}

void ASTCharacterBase::UpdateEnemyStateWidget_Multicast_Implementation(FGameplayTag StateTag, bool bShow)
{
	if (bShow)
		Delegate_OnStateAdd.Broadcast(StateTag);
	else
		Delegate_OnStateRemove.Broadcast(StateTag);
}

