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
#include "System/STGameMode_MissionBase.h"


ASTPortal::ASTPortal()
	: WC_World(nullptr), PortalType(EPortalType::None)
{
	bReplicates = true;
}

void ASTPortal::BeginPlay()
{
	Super::BeginPlay();

	//InteractWidgetClass_ForDebug change widget to show vote count

	// TSubclassOf<USTWC_LocalPlayerFacing> wcClass = LoadClass<USTWC_LocalPlayerFacing>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Sentinels/UI/Interact/Portal/WCBP/WCBP_Portal.WCBP_Portal_C'"));
	if (FacingWidgetClass)
	{
		WC_World = NewObject<USTWC_LocalPlayerFacing>(this, FacingWidgetClass);
		if (WC_World)
		{
			WC_World->RegisterComponent();
			WC_World->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}

	if (ASTGameMode_MissionBase* gamemode = Cast<ASTGameMode_MissionBase>(UGameplayStatics::GetGameMode(this)))
	{
		gamemode->Delegate_StartVoting.AddDynamic(this, &ASTPortal::OnStartVoting);
		gamemode->Delegate_VoteUpdated.AddDynamic(this, &ASTPortal::OnVoteUpdated);
	}

}

void ASTPortal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ASTPortal::OnStartVoting()
{
	// Reset Widget && Show Widget
	if (WC_World)
	{
		if (USTWidget_Portal* widget_portal = Cast<USTWidget_Portal>(WC_World->GetWidget()))
		{
			widget_portal->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			widget_portal->VoteCountUpdated(0);
		}
	}
}

void ASTPortal::OnVoteUpdated(EPortalType inPortalType, int voteCount)
{
	if (PortalType == inPortalType)
		OnVoteUpdated_Multicast(inPortalType, voteCount);
}

void ASTPortal::OnVoteUpdated_Multicast_Implementation(EPortalType inPortalType, int voteCount)
{
	if (WC_World)
	{
		if (USTWidget_Portal* widget_portal = Cast<USTWidget_Portal>(WC_World->GetWidget()))
		{
			widget_portal->VoteCountUpdated(voteCount);
		}
	}
}
