// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Dummy/STDummyPlayer.h"
#include "Net/UnrealNetwork.h"
#include "Sentinels_LS.h"
#include "Player/STPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "Components/SceneCaptureComponent2D.h"
#include "EngineUtils.h"
#include "Engine/Engine.h"
#include "Components/UI/STPlayerUIComponent.h"
#include "Materials/Material.h"
#include "TimerManager.h"
#include "Sentinels_LS.h"

// Sets default values
ASTDummyPlayer::ASTDummyPlayer() :
	bIsShow(false),
	CurrentClass(ESTClassType::GreatSword)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	SKComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh Component"));
	SKComponent->SetIsReplicated(true);

	RootComponent = SKComponent;

	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture Component"));
	//CaptureComponent->SetIsReplicated(true); // 위치 고정시킬 거니 뭐.. 굳이?

	CaptureComponent->AttachToComponent(SKComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ASTDummyPlayer::BeginPlay()
{
	Super::BeginPlay();

	OnRep_CurrentClass();
}

// Called every frame
void ASTDummyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASTDummyPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASTDummyPlayer, PlayerID);
	DOREPLIFETIME(ASTDummyPlayer, CurrentClass);
}

void ASTDummyPlayer::ServerRPCChangeCurrentClass_Implementation(ESTClassType InClass)
{
	CurrentClass = InClass;

	OnRep_CurrentClass();
}

UTextureRenderTarget2D* ASTDummyPlayer::GetTextureRenderTarget2D()
{
	return CaptureComponent->TextureTarget;
}

ASTDummyPlayer* ASTDummyPlayer::FindByID(UObject* WorldContextObject, FUniqueNetIdRepl ID)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	for (ASTDummyPlayer* dummyPlayer : TActorRange<ASTDummyPlayer>(WorldContextObject->GetWorld()))
	{
		if (*dummyPlayer->PlayerID == ID)
			return dummyPlayer;
	}

	return nullptr;
}

void ASTDummyPlayer::OnRep_PlayerID()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {

		ASTPlayerController* pc = Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		if (!pc || !pc->GetLocalPlayer())
			return;

		pc->GetUIComponent()->UpdateLoadoutUI();

	}, 0.5f, false);
}

void ASTDummyPlayer::OnRep_CurrentClass()
{
	USkeletalMesh** skMesh = SKMap.Find(CurrentClass);
	TSubclassOf<UAnimInstance>* animInstance = AIMap.Find(CurrentClass);

	if (skMesh == nullptr || animInstance == nullptr)
		return;

	SKComponent->SetSkeletalMesh(*skMesh);
	SKComponent->SetAnimInstanceClass(*animInstance);
}

