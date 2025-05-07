// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Enemy_NormalAttack.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API UBTT_Enemy_NormalAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTT_Enemy_NormalAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	FDelegateHandle LamdaHandle;

	UPROPERTY()
	bool IsMontagePlaying = false;

	UPROPERTY()
	class ASTEnemyBase* Enemy;

protected:
	void CleanUp();

	UFUNCTION()
	void OnIntrruptedCallback(class UAnimMontage* Montage, bool Interrupted);
	bool IsMontageIntrrupted = false;
};
