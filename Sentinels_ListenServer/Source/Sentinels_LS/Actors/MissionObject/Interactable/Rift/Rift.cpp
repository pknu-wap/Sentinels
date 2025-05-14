// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MissionObject/Interactable/Rift/Rift.h"
#include "Components/InteractComponent.h"
#include "Character/STCharacterBase.h"
#include "Character/Enemy/STEnemyBase.h"
#include "STGameplayTags.h"
#include "Components/SpawnEnemyComponent.h"

ARift::ARift()
{
	SpawnComponent = CreateDefaultSubobject<USpawnEnemyComponent>(FName("SpawnComponent"));
	SpawnComponent->SetShouldLoop(false);
}

void ARift::BeginPlay()
{
	Super::BeginPlay();

	bIsInteractable = false;
	SpawnComponent->Delegate_OnEnemyAllDied.AddLambda( 
		[&]()
		{
			UE_LOG(LogTemp, Display, TEXT("ARift : bIsInteractable = true "));
			bIsInteractable = true; 
		} 
	);
}

void ARift::Interact_Implementation(UInteractComponent* InteractingComponent)
{
	if (HasAuthority())
	{
		// Caching Interact Component
		InteractedComponent = InteractingComponent;

		// Start Rescue
		GetWorldTimerManager().SetTimer(Handle_Hold, this, &ARift::RepairSuccessed, InteractionHoldTime, false);

		// 1. Show Interacting Widget
		// 2. Add tag to Player
		if (InteractedComponent)
		{
			InteractedComponent->StartInteractHold_Client(InteractionHoldTime);

			APlayerController* PC = Cast<APlayerController>(InteractedComponent->GetOwner());
			if (PC)
			{
				ASTCharacterBase* Character = Cast<ASTCharacterBase>(PC->GetPawn());
				if (Character)
				{
					Character->AddTag(FSTGameplayTags::Get().Character_Player_State_RescueHostage);
				}
			}
		}
	}
}

void ARift::Interact_Finish_Implementation(UInteractComponent* InteractingComponent)
{
	if (HasAuthority())
	{
		// Clear Timer
		GetWorldTimerManager().ClearTimer(Handle_Hold);

		// 1. Hide Interacting Widget
		// 2, Remove tag from Player
		if (InteractedComponent)
		{
			InteractedComponent->FinishInteractHold_Client();

			APlayerController* PC = Cast<APlayerController>(InteractedComponent->GetOwner());
			if (PC)
			{
				ASTCharacterBase* Character = Cast<ASTCharacterBase>(PC->GetPawn());
				if (Character)
				{
					Character->RemoveTag(FSTGameplayTags::Get().Character_Player_State_RescueHostage);
				}
			}
		}
	}
}

void ARift::SpawnInitialEnemy()
{
	if (HasAuthority() && SpawnComponent)
	{
		SpawnComponent->StartSpawnEnemy();
	}
}

void ARift::RepairSuccessed()
{
	Delegate_MissionConditionUpdate.Broadcast(ObjectID, true);
	bIsInteractable = false;

	// 1. Hide Interacting Widget
	// 2, Remove tag from Player
	if (InteractedComponent)
	{
		InteractedComponent->FinishInteractHold_Client();

		APlayerController* PC = Cast<APlayerController>(InteractedComponent->GetOwner());
		if (PC)
		{
			ASTCharacterBase* Character = Cast<ASTCharacterBase>(PC->GetPawn());
			if (Character)
			{
				Character->AddTag(FSTGameplayTags::Get().Character_Player_State_RescueHostage);
			}
		}
	}
}
