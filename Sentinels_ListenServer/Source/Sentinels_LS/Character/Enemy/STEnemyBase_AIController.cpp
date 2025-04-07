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
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/STPlayerCharacter.h"

ASTEnemyBase_AIController::ASTEnemyBase_AIController(const FObjectInitializer& object_initializer)
	: Super(object_initializer)
{
	PrimaryActorTick.bStartWithTickEnabled = false;

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

	// SetAIPerception();

	if (IsValid(BehaviorTree) && IsValid(BTComponent) && IsValid(Blackboard))
	{
		RunBehaviorTree(BehaviorTree);
		BTComponent->StartTree(*BehaviorTree);
		Blackboard->SetValueAsFloat(BBKey_TargetDistance, 10000);
	}
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

void ASTEnemyBase_AIController::OnTargetDetected(AActor* actor, const FAIStimulus Stimulus)
{
	if (!IsValid(Blackboard))
	{
		UE_LOG(LogTemp, Warning, TEXT("ASTEnemyBase_AIController : Blackboard is not valid."));
		return;
	}

	if (actor == nullptr)
	{
		Blackboard->SetValueAsObject(BBKey_Target, nullptr);
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
			Blackboard->SetValueAsObject(BBKey_Target, actor);
			Blackboard->SetValueAsVector(BBKey_TargetLocation, actor->GetActorLocation());
		}
	}
	else if (Stimulus.Type == HearingConfig->GetSenseID())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s Hearing : Player Detected."), *GetName());
			Blackboard->SetValueAsObject(BBKey_Target, actor);
			Blackboard->SetValueAsVector(BBKey_TargetLocation, actor->GetActorLocation());
		}
	}
}

void ASTEnemyBase_AIController::SetTarget(AActor* InTarget)
{
	if (!IsValid(Blackboard))
	{
		UE_LOG(LogTemp, Warning, TEXT("ASTEnemyBase_AIController : Blackboard is not valid."));
		return;
	}

	if (InTarget->IsA<ASTEnemyBase>())
		return;

	AActor* currentTarget = Cast<AActor>(Blackboard->GetValueAsObject(BBKey_Target));
	if (currentTarget)
	{
		UE_LOG(LogTemp, Display, TEXT("ASTEnemyBase_AIController::SetTarget"));
		if (currentTarget->IsA<ASTPlayerCharacter>())
		{
			Blackboard->SetValueAsObject(BBKey_Target, InTarget);
		}
		else
		{
			StoredTarget = currentTarget;
			Blackboard->SetValueAsObject(BBKey_Target, InTarget);

			GetWorldTimerManager().SetTimer(StoreTargetHandle, this, &ASTEnemyBase_AIController::RestoreTarget, 30.f, false);
		}
	}
	else
	{
		Blackboard->SetValueAsObject(BBKey_Target, InTarget);
		Blackboard->SetValueAsVector(BBKey_TargetLocation, InTarget->GetActorLocation());
	}
}

void ASTEnemyBase_AIController::RestoreTarget()
{
	if(IsValid(StoredTarget) && IsValid(Blackboard))
		Blackboard->SetValueAsObject(BBKey_Target, StoredTarget);
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
