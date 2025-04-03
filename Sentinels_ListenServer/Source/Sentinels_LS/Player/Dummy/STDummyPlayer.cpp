// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Dummy/STDummyPlayer.h"
#include "Net/UnrealNetwork.h"
#include "Sentinels_LS.h"
#include "Player/STPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "Components/SceneCaptureComponent2D.h"
#include "EngineUtils.h"

// Sets default values
ASTDummyPlayer::ASTDummyPlayer()
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

	// 여기가 아니라 클라이언트가 서버에 접속할 때, 추가를 해줘야 함
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
}

void ASTDummyPlayer::UpdateSkeletalMesh(ESTClassType InClass)
{
	ST_LOG(LogSTNetwork, Warning, TEXT("Begin"));

	USkeletalMesh** skMesh = SKMap.Find(InClass);
	TSubclassOf<UAnimInstance>* animInstance = AIMap.Find(InClass);

	if (skMesh == nullptr || animInstance == nullptr)
		return;

	SKComponent->SetSkeletalMesh(*skMesh);
	SKComponent->SetAnimInstanceClass(*animInstance);
}

UTextureRenderTarget2D* ASTDummyPlayer::GetTextureRenderTarget2D()
{
	return CaptureComponent->TextureTarget;
}

ASTDummyPlayer* ASTDummyPlayer::FindByID(UWorld* World, FUniqueNetIdRepl PlayerID)
{
	if (!World) return nullptr;

	for (ASTDummyPlayer* dummyPlayer : TActorRange<ASTDummyPlayer>(World))
	{
		if (dummyPlayer->PlayerID == PlayerID)
			return dummyPlayer;
	}

	return nullptr;
}

