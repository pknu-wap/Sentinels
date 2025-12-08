// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/STEnemyBase.h"
#include "STStructs.h"
#include "STEliteBase.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API ASTEliteBase : public ASTEnemyBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	/*
		Elite Atack
	*/
public:
	int GetRandomEliteAttackMontageIndex();
	bool IsEliteAttackMontage(const UAnimMontage* InMontage) const;

	UFUNCTION(Server, Reliable)
	void ActivateEliteAttack_Server();

	UFUNCTION(NetMulticast, Reliable)
	void ActivateEliteAttack_Multicast(int MontageIdx);

	void PlayEliteAttackMontage(int MontageIdx);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TArray<UAnimMontage*> Montage_EliteAttackSet;

private:
	int LastEliteAttackMontageIndex;


	/*
		Drop Item
	*/
protected:
	virtual void DropItem() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Drop")
	FDropInfo DropInfo_Unique;
};
