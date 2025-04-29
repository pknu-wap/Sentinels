// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_GRIM_Ultimate.generated.h"

class ASTGRIM;

UCLASS()
class SENTINELS_LS_API UBTT_GRIM_Ultimate : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTT_GRIM_Ultimate();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	FDelegateHandle LamdaHandle;

	UPROPERTY()
	bool IsMontagePlaying = false;

	UPROPERTY()
	ASTGRIM* GRIM;

protected:
	void CleanUp();

	UFUNCTION()
	void OnIntrruptedCallback(class UAnimMontage* Montage, bool Interrupted);
	bool IsMontageIntrrupted = false;
};
