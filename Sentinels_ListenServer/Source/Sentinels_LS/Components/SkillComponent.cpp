// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent.h"
#include "GameFramework/Character.h"
#include "Components/STPlayerStatusComponent.h"

// Sets default values for this component's properties
USkillComponent::USkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < 4; i++)
		Skill_CoolDowns.Add(0.f);
}


// Called every frame
void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Tick_SkillCoolDown(DeltaTime);
}

void USkillComponent::InitSkillInfos(UDataTable* InDataTable)
{
	ClassSkills.Empty();

	if (InDataTable)
	{
		TArray<FSkillStruct*> SkillData;

		InDataTable->GetAllRows<FSkillStruct>(FString("USkillComponent : Get Skill Data"), SkillData);
		for (auto& SkillInfo : SkillData)
		{
			if (SkillInfo)
			{
				ClassSkills.Add(*SkillInfo);
			}
		}

		int SkillID = 0;
		for (int i = 0; i < ClassSkills.Num(); i++)
		{
			ClassSkills[i].ID = SkillID++;
		}
	}
}

void USkillComponent::ActivateSkill(int SkillIdx)
{
	StartCoolDown(SkillIdx);

	OnActivateSkill.Broadcast(SkillIdx);
}

bool USkillComponent::CanActivateSkill(int SkillIdx)
{
	// Owner Is Player Controller
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (!PC) return false;

	ACharacter* Player = PC->GetCharacter();
	if (!Player) return false;

	// If Already Playing Another Animation, return false;
	UAnimInstance* AnimInst = Player->GetMesh()->GetAnimInstance();
	if (AnimInst && AnimInst->GetCurrentActiveMontage())
		return false;

	if (Skill_CoolDowns.IsValidIndex(SkillIdx) && Skill_CoolDowns[SkillIdx] == 0)
		return true;

	return false;
}

void USkillComponent::StartCoolDown(int SkillIdx)
{
	AController* Controller = Cast<AController>(GetOwner());
	APawn* Player = Controller->GetPawn();

	USTPlayerStatusComponent* StatusComp = Player->GetComponentByClass<USTPlayerStatusComponent>();
	if (!StatusComp) return;

	if (Skill_CoolDowns.IsValidIndex(SkillIdx) && ClassSkills.IsValidIndex(SkillIdx))
		Skill_CoolDowns[SkillIdx] = ClassSkills[SkillIdx].SkillCoolTime * (1 - StatusComp->CDR);
}

void USkillComponent::Tick_SkillCoolDown(float DeltaTime)
{
	for (int i = 0; i < Skill_CoolDowns.Num(); i++)
	{
		if (Skill_CoolDowns[i] > 0.f)
		{
			Skill_CoolDowns[i] = FMath::Clamp(Skill_CoolDowns[i] - DeltaTime, 0.f, 50.f);
		}
	}
}

