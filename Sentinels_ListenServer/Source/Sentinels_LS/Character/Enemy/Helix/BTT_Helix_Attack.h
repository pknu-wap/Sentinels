// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "STEnums.h"
#include "BTT_Helix_Attack.generated.h"


UCLASS()
class SENTINELS_LS_API UBTT_Helix_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_Helix_Attack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EHelixAttackType> AttackType;

private:
	FDelegateHandle LamdaHandle;

	UPROPERTY()
	bool IsMontagePlaying = false;

	UPROPERTY()
	class ASTHelix* Helix;

protected:
	void CleanUp();

	UFUNCTION()
	void OnIntrruptedCallback(class UAnimMontage* Montage, bool Interrupted);
	bool IsMontageIntrrupted = false;
};
