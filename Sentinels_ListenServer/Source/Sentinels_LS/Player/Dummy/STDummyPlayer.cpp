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
#include "SubSystem/STGameTravelDataSubsystem.h"
#include "STStructs.h"

// Sets default values
ASTDummyPlayer::ASTDummyPlayer() :
	CurrentClass(ESTClassType::GreatSword),
	Name_Head("0"),
	Name_Hood("0"),
	Name_LongHair("0"),
	Name_Glasses("0"),
	Name_UpperBody("0"),
	Name_Backpack("0"),
	Name_Hand_L("0"),
	Name_Hand_R("0"),
	Name_BottomBody("0"),
	Name_Feet("0"),
	Name_GreatSword("0"),
	Name_Katana("0"),
	Name_Blade_L("0"),
	Name_Blade_R("0")
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	SKMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));
	RootComponent = SKMeshComponent;
	SKMeshComponent->SetIsReplicated(true);

	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture Component"));
	CaptureComponent->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);

	HeadComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	HeadComponent->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HoodComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hood"));
	HoodComponent->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	LongHairComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LongHair"));
	LongHairComponent->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	GlassesComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Glassess"));
	GlassesComponent->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	UpperBodyComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("UpperBody"));
	UpperBodyComponent->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	BackpackComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Backpack"));
	BackpackComponent->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Hand_LComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hand_L"));
	Hand_LComponent->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Hand_RComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hand_R"));
	Hand_RComponent->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	BottomBodyComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BottomBody"));
	BottomBodyComponent->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FeetComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Feet"));
	FeetComponent->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	GreatSwordComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GreatSword"));
	GreatSwordComponent->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Weapon_R"));
	KatanaComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Katana"));
	KatanaComponent->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Weapon_R"));
	Blade_LComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Blade_L"));
	Blade_LComponent->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, ("Weapon_L"));
	Blade_RComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Blade_R"));
	Blade_RComponent->AttachToComponent(SKMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Weapon_R"));
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
	DOREPLIFETIME(ASTDummyPlayer, PlayerName);
	DOREPLIFETIME(ASTDummyPlayer, CurrentClass);
	DOREPLIFETIME(ASTDummyPlayer, Name_Head);
	DOREPLIFETIME(ASTDummyPlayer, Name_Hood);
	DOREPLIFETIME(ASTDummyPlayer, Name_LongHair);
	DOREPLIFETIME(ASTDummyPlayer, Name_Glasses);
	DOREPLIFETIME(ASTDummyPlayer, Name_UpperBody);
	DOREPLIFETIME(ASTDummyPlayer, Name_Backpack);
	DOREPLIFETIME(ASTDummyPlayer, Name_Hand_L);
	DOREPLIFETIME(ASTDummyPlayer, Name_Hand_R);
	DOREPLIFETIME(ASTDummyPlayer, Name_BottomBody);
	DOREPLIFETIME(ASTDummyPlayer, Name_Feet);
	DOREPLIFETIME(ASTDummyPlayer, Name_GreatSword);
	DOREPLIFETIME(ASTDummyPlayer, Name_Katana);
	DOREPLIFETIME(ASTDummyPlayer, Name_Blade_L);
	DOREPLIFETIME(ASTDummyPlayer, Name_Blade_R);
}

void ASTDummyPlayer::ServerRPCChangeCurrentClass_Implementation(ESTClassType InClass)
{
	CurrentClass = InClass;

	USTGameTravelDataSubsystem* gameTravelDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USTGameTravelDataSubsystem>();
	if (!gameTravelDataSubsystem)
		return;

	gameTravelDataSubsystem->SavePlayerCurrentClass(PlayerID, CurrentClass);

	OnRep_CurrentClass();
}

void ASTDummyPlayer::ServerRPCChangeSKMesh_Implementation(ESKParts Part, FName SKMeshRowName)
{
	USTGameTravelDataSubsystem* gameTravelDataSubsystem = GetGameInstance()->GetSubsystem<USTGameTravelDataSubsystem>();
	if (!gameTravelDataSubsystem)
		return;

	FPlayerInfo playerInfo = gameTravelDataSubsystem->LoadPlayerInfo(PlayerID);
	FPlayerSKMeshesRowName playerSKMeshesRowName = playerInfo.PlayerSKMeshesRowName;

	switch (Part)
	{
	case ESKParts::Head:
		playerSKMeshesRowName.Name_Head = SKMeshRowName;
		break;
	case ESKParts::Hood:
		playerSKMeshesRowName.Name_Hood = SKMeshRowName;
		break;
	case ESKParts::LongHair:
		playerSKMeshesRowName.Name_LongHair = SKMeshRowName;
		break;
	case ESKParts::Glasses:
		playerSKMeshesRowName.Name_Glasses = SKMeshRowName;
		break;
	case ESKParts::UpperBody:
		playerSKMeshesRowName.Name_UpperBody = SKMeshRowName;
		break;
	case ESKParts::Backpack:
		playerSKMeshesRowName.Name_Backpack = SKMeshRowName;
		break;
	case ESKParts::Hand_L:
		playerSKMeshesRowName.Name_Hand_L = SKMeshRowName;
		break;
	case ESKParts::Hand_R:
		playerSKMeshesRowName.Name_Hand_R = SKMeshRowName;
		break;
	case ESKParts::BottomBody:
		playerSKMeshesRowName.Name_BottomBody = SKMeshRowName;
		break;
	case ESKParts::Feet:
		playerSKMeshesRowName.Name_Feet = SKMeshRowName;
		break;
	}

	gameTravelDataSubsystem->SavePlayerSKMeshes(PlayerID, playerSKMeshesRowName);
	ChangeSKMeshName(Part, SKMeshRowName);
}

void ASTDummyPlayer::ChangeSKMeshName(ESKParts Part, FName SKMeshRowName)
{
	ST_LOG(LogSTNetwork, Log, TEXT("ChangeSKMeshName"));

	switch (Part)
	{
	case ESKParts::Head:
		Name_Head = SKMeshRowName;
		break;
	case ESKParts::Hood:
		Name_Hood = SKMeshRowName;
		break;
	case ESKParts::LongHair:
		Name_LongHair = SKMeshRowName;
		break;
	case ESKParts::Glasses:
		Name_Glasses = SKMeshRowName;
		break;
	case ESKParts::UpperBody:
		Name_UpperBody = SKMeshRowName;
		break;
	case ESKParts::Backpack:
		Name_Backpack = SKMeshRowName;
		break;
	case ESKParts::Hand_L:
		Name_Hand_L = SKMeshRowName;
		break;
	case ESKParts::Hand_R:
		Name_Hand_R = SKMeshRowName;
		break;
	case ESKParts::BottomBody:
		Name_BottomBody = SKMeshRowName;
		break;
	case ESKParts::Feet:
		Name_Feet = SKMeshRowName;
		break;
	}

	OnRep_SKName();
}

void ASTDummyPlayer::OnRep_PlayerNames()
{
	UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *PlayerName);
}

UTextureRenderTarget2D* ASTDummyPlayer::GetTextureRenderTarget2D()
{
	return CaptureComponent->TextureTarget;
}

USkeletalMeshComponent* ASTDummyPlayer::GetSKMeshComponent(ESKParts Part)
{
	switch (Part)
	{
	case ESKParts::Head:
		return HeadComponent;
	case ESKParts::Hood:
		return HoodComponent;
	case ESKParts::LongHair:
		return LongHairComponent;
	case ESKParts::Glasses:
		return GlassesComponent;
	case ESKParts::UpperBody:
		return UpperBodyComponent;
	case ESKParts::Backpack:
		return BackpackComponent;
	case ESKParts::Hand_L:
		return Hand_LComponent;
	case ESKParts::Hand_R:
		return Hand_RComponent;
	case ESKParts::BottomBody:
		return BottomBodyComponent;
	case ESKParts::Feet:
		return FeetComponent;
	case ESKParts::GreatSword:
		return GreatSwordComponent;
	case ESKParts::Katana:
		return KatanaComponent;
	case ESKParts::Blade_L:
		return Blade_LComponent;
	case ESKParts::Blade_R:
		return Blade_RComponent;
	default:
		return HeadComponent;
	}
}

void ASTDummyPlayer::OnRep_PlayerID()
{
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, [this]() {

		ASTPlayerController* pc = Cast<ASTPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		if (!pc || !pc->GetLocalPlayer())
			return;

		pc->GetUIComponent()->UpdateUI(FSTGameplayTags::Get().Widget_Lobby_Loadout);
		OnRep_CurrentClass();

	}, 0.5f, false);
}

void ASTDummyPlayer::OnRep_CurrentClass()
{
	USkeletalMesh** weapon = WeaponMap.Find(CurrentClass);
	TSubclassOf<UAnimInstance>* animInstance = AIMap.Find(CurrentClass);

	if (weapon == nullptr || animInstance == nullptr)
		return;

	GreatSwordComponent->SetSkeletalMesh(nullptr);
	KatanaComponent->SetSkeletalMesh(nullptr);
	Blade_LComponent->SetSkeletalMesh(nullptr);
	Blade_RComponent->SetSkeletalMesh(nullptr);

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

void ASTDummyPlayer::OnRep_SKName()
{
	USTGameTravelDataSubsystem* gameTravelDataSubsystem = GetGameInstance()->GetSubsystem<USTGameTravelDataSubsystem>();
	if (!gameTravelDataSubsystem)
		return;

	HeadComponent->SetSkeletalMesh(gameTravelDataSubsystem->GetSKMeshDT(ESKParts::Head)->FindRow<FCustomizeStruct>(Name_Head, "")->SkeletalMesh);
	HoodComponent->SetSkeletalMesh(gameTravelDataSubsystem->GetSKMeshDT(ESKParts::Hood)->FindRow<FCustomizeStruct>(Name_Hood, "")->SkeletalMesh);
	LongHairComponent->SetSkeletalMesh(gameTravelDataSubsystem->GetSKMeshDT(ESKParts::LongHair)->FindRow<FCustomizeStruct>(Name_LongHair, "")->SkeletalMesh);
	GlassesComponent->SetSkeletalMesh(gameTravelDataSubsystem->GetSKMeshDT(ESKParts::Glasses)->FindRow<FCustomizeStruct>(Name_Glasses, "")->SkeletalMesh);
	UpperBodyComponent->SetSkeletalMesh(gameTravelDataSubsystem->GetSKMeshDT(ESKParts::UpperBody)->FindRow<FCustomizeStruct>(Name_UpperBody, "")->SkeletalMesh);
	BackpackComponent->SetSkeletalMesh(gameTravelDataSubsystem->GetSKMeshDT(ESKParts::Backpack)->FindRow<FCustomizeStruct>(Name_Backpack, "")->SkeletalMesh);
	Hand_LComponent->SetSkeletalMesh(gameTravelDataSubsystem->GetSKMeshDT(ESKParts::Hand_L)->FindRow<FCustomizeStruct>(Name_Hand_L, "")->SkeletalMesh);
	Hand_RComponent->SetSkeletalMesh(gameTravelDataSubsystem->GetSKMeshDT(ESKParts::Hand_R)->FindRow<FCustomizeStruct>(Name_Hand_R, "")->SkeletalMesh);
	BottomBodyComponent->SetSkeletalMesh(gameTravelDataSubsystem->GetSKMeshDT(ESKParts::BottomBody)->FindRow<FCustomizeStruct>(Name_BottomBody, "")->SkeletalMesh);
	FeetComponent->SetSkeletalMesh(gameTravelDataSubsystem->GetSKMeshDT(ESKParts::Feet)->FindRow<FCustomizeStruct>(Name_Feet, "")->SkeletalMesh);
}
