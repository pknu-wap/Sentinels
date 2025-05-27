// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/STEnemyBase_AIController.h"
#include "Character/Enemy/STEnemyBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/STPlayerCharacter.h"

ASTEnemyBase_AIController::ASTEnemyBase_AIController(const FObjectInitializer& object_initializer)
	: Super(object_initializer)
{
	PrimaryActorTick.bStartWithTickEnabled = true;

	BTComponent = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	Blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	HearingConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));
	SetAIPerception();
}

void ASTEnemyBase_AIController::BeginPlay()
{
	Super::BeginPlay();
}

void ASTEnemyBase_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Cast<UBehaviorTreeComponent>(BrainComponent)
	if (IsValid(Blackboard) && IsValid(BehaviorTree))
	{
		Blackboard->InitializeBlackboard(*BehaviorTree->GetBlackboardAsset());
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("ASTEnemyBase_AIController : Blackboard is not valid."));
}

void ASTEnemyBase_AIController::StartAILogic()
{
	if (IsValid(BehaviorTree) && IsValid(BTComponent) && IsValid(Blackboard))
	{
		RunBehaviorTree(BehaviorTree);
		BTComponent->StartTree(*BehaviorTree);
		Blackboard->SetValueAsFloat(BBKey_TargetDistance, 10000);
		Blackboard->SetValueAsFloat(BBKey_Time_Stunned, 1.f);

		APawn* pawn = GetPawn();
		if (pawn)
		{
			Blackboard->SetValueAsVector(FName("StartLocation"), pawn->GetActorLocation());
		}
	}
}

void ASTEnemyBase_AIController::OnTargetDetected(AActor* actor, const FAIStimulus Stimulus)
{
	if (!IsValid(Blackboard))
	{
		UE_LOG(LogTemp, Warning, TEXT("ASTEnemyBase_AIController : Blackboard is not valid."));
		return;
	}

	UObject* target = Blackboard->GetValueAsObject(BBKey_Target);
	if (target)
		return;

	if (!actor->IsA<ASTPlayerCharacter>()) return;

	if (Stimulus.Type == SightConfig->GetSenseID())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s Sight : Player Detected."), *GetName());
			SetTarget(actor);
		}
	}
	else if (Stimulus.Type == HearingConfig->GetSenseID())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s Hearing : Player Detected."), *GetName());
			SetTarget(actor);
		}
	}
}

AActor* ASTEnemyBase_AIController::GetCurrentTarget() const
{
	if (IsValid(Blackboard))
	{
		return Cast<AActor>(Blackboard->GetValueAsObject(BBKey_Target));
	}

	return nullptr;
}

void ASTEnemyBase_AIController::SetTarget(AActor* InTarget)
{
	if (!IsValid(Blackboard))
	{
		UE_LOG(LogTemp, Warning, TEXT("ASTEnemyBase_AIController : Blackboard is not valid."));
		return;
	}

	if (!InTarget)
	{
		UE_LOG(LogTemp, Display, TEXT("%s Set Target as nullptr"), *GetName());
		Blackboard->SetValueAsObject(BBKey_Target, nullptr);
		return;
	}
	else if (!InTarget->IsA<ASTPlayerCharacter>())
		return;

	// Store Target if New Target is not player
	AActor* currentTarget = Cast<AActor>(Blackboard->GetValueAsObject(BBKey_Target));
	if (currentTarget && !currentTarget->IsA<ASTPlayerCharacter>())
	{
		StoredTarget = currentTarget;
		GetWorldTimerManager().SetTimer(StoreTargetHandle, this, &ASTEnemyBase_AIController::RestoreTarget, 30.f, false);
	}

	// Set Target
	UE_LOG(LogTemp, Display, TEXT("%s Set Target as %s"), *(GetName()), *(InTarget->GetName()));
	Blackboard->SetValueAsObject(BBKey_Target, InTarget);
}

void ASTEnemyBase_AIController::RestoreTarget()
{
	if(IsValid(StoredTarget) && IsValid(Blackboard))
		Blackboard->SetValueAsObject(BBKey_Target, StoredTarget);
}

void ASTEnemyBase_AIController::ApplyStun(float StunTime)
{
	if (IsValid(Blackboard))
	{
		float currentStunTime = Blackboard->GetValueAsFloat(BBKey_Time_Stunned);

		if (currentStunTime == StunTime)
		{
			StunTime += 0.01;
		}

		Blackboard->SetValueAsFloat(BBKey_Time_Stunned, StunTime);
		Blackboard->SetValueAsBool(BBKey_Stunned, true);
	}
}

void ASTEnemyBase_AIController::SetAIPerception()
{
	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = AILastSeenLocation;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	HearingConfig->HearingRange = 3000.0f;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->SetMaxAge(35.0f);

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ASTEnemyBase_AIController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	GetPerceptionComponent()->ConfigureSense(*HearingConfig);
}
