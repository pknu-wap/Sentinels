// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent.h"

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

	FSkillStruct SkillInfo;
	SkillInfo.SkillCoolTime = 5.f;
	for (int i = 1; i <= 4; i++)
	{
		SkillInfo.ID = i;
		ClassSkills.Add(SkillInfo);
	}
}


// Called every frame
void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Tick_SkillCoolDown(DeltaTime);
}

void USkillComponent::ActivateSkill(int SkillIdx)
{
	StartCoolDown(SkillIdx);
}

bool USkillComponent::CanActivateSkill(int SkillIdx)
{
	if (Skill_CoolDowns.IsValidIndex(SkillIdx) && Skill_CoolDowns[SkillIdx] == 0)
		return true;

	return false;
}

void USkillComponent::StartCoolDown(int SkillIdx)
{
	if (Skill_CoolDowns.IsValidIndex(SkillIdx) && ClassSkills.IsValidIndex(SkillIdx))
		Skill_CoolDowns[SkillIdx] = ClassSkills[SkillIdx].SkillCoolTime;
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

