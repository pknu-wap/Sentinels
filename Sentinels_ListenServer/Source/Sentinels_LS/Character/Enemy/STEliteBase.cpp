// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/STEliteBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/Interact/Item/InteractableItem.h"

int ASTEliteBase::GetRandomEliteAttackMontageIndex()
{
	const int32 NumMontages = Montage_EliteAttackSet.Num();

	if (NumMontages <= 1)
		return 0; // 하나뿐이면 어쩔 수 없음

	int32 NewIndex;
	do
	{
		NewIndex = FMath::RandRange(0, NumMontages - 1);
	} while (NewIndex == LastEliteAttackMontageIndex);

	LastEliteAttackMontageIndex = NewIndex;
	return NewIndex;
}

bool ASTEliteBase::IsEliteAttackMontage(const UAnimMontage* InMontage) const
{
	for (int i = 0; i < Montage_EliteAttackSet.Num(); i++)
	{
		if (Montage_EliteAttackSet[i] == InMontage)
			return true;
	}
	return false;
}

void ASTEliteBase::ActivateEliteAttack_Server_Implementation()
{
	int MontageIdx = GetRandomEliteAttackMontageIndex();
	ActivateEliteAttack_Multicast(MontageIdx);
	PlayEliteAttackMontage(MontageIdx);
}

void ASTEliteBase::ActivateEliteAttack_Multicast_Implementation(int MontageIdx)
{
	if (!HasAuthority())
		PlayEliteAttackMontage(MontageIdx);
}

void ASTEliteBase::PlayEliteAttackMontage(int MontageIdx)
{
	if (!Montage_EliteAttackSet.IsValidIndex(MontageIdx))
		return;

	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
	{
		AnimInst->Montage_Play(Montage_EliteAttackSet[MontageIdx]);
	}
}

void ASTEliteBase::DropItem()
{
	if (HasAuthority())
	{
		float rand = UKismetMathLibrary::RandomFloatInRange(0, 1);

		if (DropInfo_Unique.DropProbability >= rand)
		{
			GetWorld()->SpawnActor<AActor>(DropInfo_Unique.DropItemClass, GetActorLocation(), GetActorRotation());
		}

		for (const auto& dropInfo : DropInfos_Additional)
		{
			rand = UKismetMathLibrary::RandomFloatInRange(0, 1);
			if (dropInfo.DropProbability >= rand)
			{
				GetWorld()->SpawnActor<AActor>(dropInfo.DropItemClass, GetActorLocation(), GetActorRotation());
			}
		}
	}
}
