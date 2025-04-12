// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "STEnemyBase_AIController.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API ASTEnemyBase_AIController : public AAIController
{
	GENERATED_BODY()

public:
	const FName BBKey_Target = FName("Target");
	const FName BBKey_TargetLocation = FName("TargetLocation");
	const FName BBKey_TargetDistance = FName("TargetDistance");
	const FName BBKey_Stunned = FName("Stunned");
	const FName BBKey_Time_Stunned = FName("Time_Stunned");
	const FName BBKey_bShouldReturn = FName("bShouldReturn");
	const FName BBKey_StartLocation = FName("StartLocation");

public:
	ASTEnemyBase_AIController(const FObjectInitializer& object_initializer);

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BTComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;
	class UBlackboardData* BBData;
	class UAISenseConfig_Sight* SightConfig;
	class UAISenseConfig_Hearing* HearingConfig;

public:
	UFUNCTION()
	virtual void OnTargetDetected(AActor* actor, const FAIStimulus Stimulus);

	// Target
	void SetTarget(AActor* InTarget);
	void RestoreTarget();

	// Stunned
	void ApplyStun(float StunTime);

protected:
	FTimerHandle StoreTargetHandle;

	UPROPERTY()
	AActor* StoredTarget;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AISightRadius = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AILoseSightRadius = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AIFieldOfView = 145.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AISightAge = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AILastSeenLocation = 900.f;

public:
	void SetAIPerception();
};
