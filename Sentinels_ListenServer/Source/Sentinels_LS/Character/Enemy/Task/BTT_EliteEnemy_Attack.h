// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_EliteEnemy_Attack.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API UBTT_EliteEnemy_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTT_EliteEnemy_Attack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	FDelegateHandle LamdaHandle;

	UPROPERTY()
	bool IsMontagePlaying = false;

	UPROPERTY()
	class ASTEliteBase* EliteEnemy;

protected:
	void CleanUp();

	UFUNCTION()
	void OnIntrruptedCallback(class UAnimMontage* Montage, bool Interrupted);
	bool IsMontageIntrrupted = false;
};
