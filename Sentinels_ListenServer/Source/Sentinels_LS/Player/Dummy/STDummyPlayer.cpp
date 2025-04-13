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
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ASTDummyPlayer::ASTDummyPlayer() :
	bIsShow(false),
	CurrentClass(ESTClassType::GreatSword)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	SKMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));
	RootComponent = SKMeshComponent;
	SKMeshComponent->SetIsReplicated(true);

	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture Component"));
	CaptureComponent->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);

	for (uint8 i = 0; i <= static_cast<uint8>(ESKParts::Feet); i++)
	{
		FName componentName = *FString::Printf(TEXT("SkeletalMeshComponent_%d"), i);
		AddSKComponents(componentName);
	}

	FName greatSword = *FString::Printf(TEXT("GreatSword"));
	AddSKComponents(greatSword);

	FName katana = *FString::Printf(TEXT("Katana"));
	AddSKComponents(katana);

	FName blade_L = *FString::Printf(TEXT("Blade_L"));
	AddSKComponents(blade_L);

	FName blade_R = *FString::Printf(TEXT("Blade_R"));
	AddSKComponents(blade_R);

	GetSKMeshComponent(ESKParts::GreatSword)->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Weapon_R"));
	GetSKMeshComponent(ESKParts::Katana)->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Weapon_R"));
	GetSKMeshComponent(ESKParts::Blade_L)->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Weapon_L"));
	GetSKMeshComponent(ESKParts::Blade_R)->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Weapon_R"));
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

USkeletalMeshComponent* ASTDummyPlayer::GetSKMeshComponent(ESKParts partName)
{
	return SKMeshComponents[static_cast<uint8>(partName)];
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

void ASTDummyPlayer::AddSKComponents(FName ComponentName)
{
	USkeletalMeshComponent* newComp = CreateDefaultSubobject<USkeletalMeshComponent>(ComponentName);
	newComp->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	newComp->SetIsReplicated(true);
	SKMeshComponents.Add(newComp);
}

void ASTDummyPlayer::OnRep_PlayerID()
{
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, [this]() {

		ASTPlayerController* pc = Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		if (!pc || !pc->GetLocalPlayer())
			return;

		pc->GetUIComponent()->UpdateLoadoutUI();

	}, 0.5f, false);
}

void ASTDummyPlayer::OnRep_CurrentClass()
{
	USkeletalMesh** weapon = WeaponMap.Find(CurrentClass);
	TSubclassOf<UAnimInstance>* animInstance = AIMap.Find(CurrentClass);

	if (weapon == nullptr || animInstance == nullptr)
		return;

	for (int i = static_cast<uint8>(ESKParts::GreatSword); i <= static_cast<uint8>(ESKParts::Blade_R); i++)
		SKMeshComponents[i]->SetSkeletalMesh(nullptr);

	switch (CurrentClass)
	{
	case ESTClassType::GreatSword:
		GetSKMeshComponent(ESKParts::GreatSword)->SetSkeletalMesh(*weapon);
		break;
	case ESTClassType::Katana:
		GetSKMeshComponent(ESKParts::Katana)->SetSkeletalMesh(*weapon);
		break;
	case ESTClassType::DualBlade:
		GetSKMeshComponent(ESKParts::Blade_L)->SetSkeletalMesh(*weapon);
		GetSKMeshComponent(ESKParts::Blade_R)->SetSkeletalMesh(*weapon);
		break;
	case ESTClassType::Magician:
		break;
	}

	SKMeshComponent->SetAnimInstanceClass(*animInstance);
}

