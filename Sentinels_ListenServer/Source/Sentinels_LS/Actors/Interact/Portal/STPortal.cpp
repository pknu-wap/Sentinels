// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interact/Portal/STPortal.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Player/STPlayerController.h"
#include "Player/STPlayerCharacter.h"
#include "Components/InteractComponent.h"
#include "STStructs.h"
#include "UI/WidgetComponent/STWC_LocalPlayerFacing.h"
#include "UI/Widget/STWidget_Portal.h"

ASTPortal::ASTPortal()
	: CurrentVoteCount(0)
{
	bReplicates = true;
}

void ASTPortal::BeginPlay()
{
	Super::BeginPlay();

	//InteractWidgetClass_ForDebug change widget to show vote count

	TSubclassOf<USTWC_LocalPlayerFacing> wcClass = LoadClass<USTWC_LocalPlayerFacing>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Sentinels/UI/Interact/Portal/WCBP/WCBP_Portal.WCBP_Portal_C'"));
	if (wcClass)
	{
		WC_World = NewObject<USTWC_LocalPlayerFacing>(this, wcClass);
		if (WC_World)
		{
			WC_World->RegisterComponent();
			WC_World->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

void ASTPortal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASTPortal, CurrentVoteCount);
}

void ASTPortal::Interact_Implementation(UInteractComponent* InteractingComponent)
{
	ASTPlayerController* pc = Cast<ASTPlayerController>(InteractingComponent->GetOwner());
	if (!pc)
		return;

	ASTPlayerCharacter* player = Cast<ASTPlayerCharacter>(pc->GetCharacter());
	if (!player)
		return;

	if (LevelHandle_DataRow.DataTable)
	{
		FPortalInfo* portalInfo = LevelHandle_DataRow.DataTable->FindRow<FPortalInfo>(LevelHandle_DataRow.RowName, LevelHandle_DataRow.RowName.ToString());
		if (!portalInfo)
			return;

		Teleport(player, portalInfo->ArrivalPoint);
	}

	MulticastRPCUpdate();
}

void ASTPortal::Interact_Finish_Implementation(UInteractComponent* InteractingComponent)
{
}

void ASTPortal::ShowInteractiveUI_Implementation(UInteractComponent* InteractingComponent)
{
	// show vote count
	// ex) 3/5
	if (!bIsInteractable)
		return;

	if (InteractWidget_ForDebug)
	{
		InteractWidget_ForDebug->RemoveFromParent();
	}

	if (InteractWidgetClass_ForDebug)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		if (PC)
		{
			InteractWidget_ForDebug = CreateWidget<UUserWidget>(PC, InteractWidgetClass_ForDebug);
			if (InteractWidget_ForDebug)
			{
				InteractWidget_ForDebug->AddToViewport();
			}
		}
	}
}

void ASTPortal::HideInteractiveUI_Implementation(UInteractComponent* InteractingComponent)
{
	Super::HideInteractiveUI_Implementation(InteractingComponent);
	if (InteractWidget_ForDebug)
	{
		InteractWidget_ForDebug->RemoveFromParent();
	}
}

void ASTPortal::Activate()
{
	Mesh->SetVisibility(true, true);
	bIsInteractable = true;
}

void ASTPortal::Deactivate()
{
	Mesh->SetVisibility(false, true);
	bIsInteractable = false;
}

void ASTPortal::Teleport(AActor* Target, FVector& Location)
{
	Target->SetActorLocation(Location, false);
}

void ASTPortal::MulticastRPCUpdate_Implementation()
{
	if (WC_World)
	{
		USTWidget_Portal* widget_portal = Cast<USTWidget_Portal>(WC_World->GetWidget());
		if (widget_portal)
		{
			widget_portal->SetTB_VoteCount();
		}
	}
}

void ASTPortal::ServerRPCVote_Implementation()
{
}