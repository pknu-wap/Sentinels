// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STPlayerStatusComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/UI/STPlayerUIComponent.h"
#include "Player/STPlayerCharacter.h"
#include "SubSystem/InventorySubsystem.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/Inventory/ItemObject.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"


// Sets default values for this component's properties
USTPlayerStatusComponent::USTPlayerStatusComponent()
	:	MaxHP(1000.f), BaseMaxHP(100.f), 
		MaxMovementSpeed(1800.f), BaseMovementSpeed(600.f),
		MaxAttackSpeed(5.f), BaseAttackSpeed(1.f),
		MaxCriticalDamagePercent(5.f), BaseCriticalDamagePercent(2.f),
		MaxCriticalRate(1.0f), BaseCriticalRate(0.1f),
		MaxDamageIncreaseRate(3.f),
		MaxCDR(0.5f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USTPlayerStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedPlayer = Cast<ASTPlayerCharacter>(GetOwner());
	if (CachedPlayer)
	{
		CachedInventory = GetOwner()->GetComponentByClass<UInventoryComponent>();
		BaseDamageType = CachedPlayer->BaseDamageType;
		CriticalDamageType = CachedPlayer->CriticalDamageType;
	}

	SetDefaultStatus();
	ApplyStatus();

	HP = MaxHP;
	OnRep_HPUpdated();

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		FTimerHandle HPRegenHandle;
		GetWorld()->GetTimerManager().SetTimer(HPRegenHandle, 
			this, &USTPlayerStatusComponent::RegenHP,
			1.f, true);
	}
}

void USTPlayerStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*
	*	Replicate to All Clients
	*/
	DOREPLIFETIME(USTPlayerStatusComponent, MaxHP);
	DOREPLIFETIME(USTPlayerStatusComponent, HP);

	DOREPLIFETIME(USTPlayerStatusComponent, MaxAttackSpeed);
	DOREPLIFETIME(USTPlayerStatusComponent, AttackSpeed);
	DOREPLIFETIME(USTPlayerStatusComponent, BaseAttackSpeed);

	/*
		Replicate to Only for Owner
	*/
	DOREPLIFETIME_CONDITION(USTPlayerStatusComponent, HPRegen, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(USTPlayerStatusComponent, ATK, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(USTPlayerStatusComponent, DEF, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(USTPlayerStatusComponent, MaxMovementSpeed, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(USTPlayerStatusComponent, MovementSpeed, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(USTPlayerStatusComponent, MaxCriticalDamagePercent, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(USTPlayerStatusComponent, CriticalDamagePercent, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(USTPlayerStatusComponent, MaxCriticalRate, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(USTPlayerStatusComponent, CriticalRate, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(USTPlayerStatusComponent, MaxDamageIncreaseRate, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(USTPlayerStatusComponent, DamageIncreaseRate, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(USTPlayerStatusComponent, MaxCDR, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(USTPlayerStatusComponent, CDR, COND_OwnerOnly);
}

float USTPlayerStatusComponent::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HP = FMath::Clamp(HP - DamageAmount, 0, MaxHP);
	OnRep_HPUpdated();

	return HP;
}

FSTDamageInfo USTPlayerStatusComponent::GetCalculatedDamageInfo(FSTPointDamageEvent& DamageEvent, AActor* DamagedActor)
{
	FSTDamageInfo DamageInfo; 

	DamageInfo.bIsCritical = DamageEvent.bIsCritical = UKismetMathLibrary::RandomFloatInRange(0, 1) <= CriticalRate ? true : false;
	if (DamageInfo.bIsCritical)
	{
		DamageInfo.DamageAmount = GetCriticalBaseDamage();
	}
	else
	{
		DamageInfo.DamageAmount = GetBaseDamage();
	}

	if (CachedInventory)
	{
		DamageInfo.DamageAmount = CachedInventory->AdjustFinalDamage(DamageInfo.DamageAmount, DamageEvent, DamagedActor);
	}

	if (CachedPlayer)
	{
		CachedPlayer->AdjustFinalDamage(DamageInfo.DamageAmount, DamageEvent, DamagedActor);
	}

	return DamageInfo;
}

float USTPlayerStatusComponent::GetBaseDamage() const
{
	UE_LOG(LogTemp, Display, TEXT("Damage : %f = %f * (1 + %f)"), ATK * (1 + DamageIncreaseRate), ATK, (1 + DamageIncreaseRate));
	return ATK * (1 + DamageIncreaseRate);
}

float USTPlayerStatusComponent::GetCriticalBaseDamage() const
{
	UE_LOG(LogTemp, Display, TEXT("Critical Damage : %f = %f * %f * (1 + %f)"), ATK * (1 + DamageIncreaseRate), ATK, CriticalDamagePercent, (1 + DamageIncreaseRate));
	return ATK * CriticalDamagePercent * (1 + DamageIncreaseRate);
}

void USTPlayerStatusComponent::SetStatus_Server_Implementation(ESTStatusType inType, float inValue, bool forceApply)
{
	if (forceApply)
	{
		switch (inType)
		{
		case ESTStatusType::HP:
			HP = inValue;
			break;
		case ESTStatusType::ATK:
			ATK = inValue;
			break;
		case ESTStatusType::DEF:
			DEF = inValue;
			break;
		case ESTStatusType::MovementSpeed:
			MovementSpeed = inValue;
			break;
		case ESTStatusType::AttackSpeed:
			AttackSpeed = inValue;
			break;
		case ESTStatusType::DamageIncreaseRate:
			DamageIncreaseRate = inValue;
			break;
		case ESTStatusType::CriticalDamagePercent:
			CriticalDamagePercent = inValue;
			break;
		case ESTStatusType::CriticalRate:
			CriticalRate = inValue;
			break;
		case ESTStatusType::CDR:
			CDR = inValue;
			break;
		default:
			break;
		}

		return;
	}

	switch (inType)
	{
	case ESTStatusType::HP:
		HP = FMath::Clamp(inValue, 0.f, MaxHP);
		break;
	case ESTStatusType::ATK:
		ATK = inValue;
		break;
	case ESTStatusType::DEF:
		DEF = inValue;
		break;
	case ESTStatusType::MovementSpeed:
		MovementSpeed = FMath::Clamp(inValue, 0.f, MaxMovementSpeed);
		break;
	case ESTStatusType::AttackSpeed:
		AttackSpeed = FMath::Clamp(inValue, 1.f, AttackSpeed);
		break;
	case ESTStatusType::DamageIncreaseRate:
		DamageIncreaseRate = FMath::Clamp(inValue, 0.f, MaxDamageIncreaseRate);
		break;
	case ESTStatusType::CriticalDamagePercent:
		CriticalDamagePercent = FMath::Clamp(inValue, 0.f, MaxCriticalDamagePercent);
		break;
	case ESTStatusType::CriticalRate:
		CriticalRate = FMath::Clamp(inValue, 0.f, MaxCriticalRate);
		break;
	case ESTStatusType::CDR:
		CDR = FMath::Clamp(inValue, 0.f, MaxCDR);
		break;
	default:
		break;
	}

	ApplyStatus();
}

void USTPlayerStatusComponent::ApplyBuff_Server_Implementation(ESTBuffType inType, float inValue, float inDuration)
{
	// Already exist buff
	FSTBuffStruct* appliedBuff = BuffMap.Find(inType);
	if (appliedBuff)
	{
		if (inValue > appliedBuff->Value)
		{
			GetWorld()->GetTimerManager().SetTimer(appliedBuff->Handle_Buff,
				[this, inType]()
				{
					this->ClearBuff_Server(inType);
				},
				inDuration, false);
		}
	}
	else
	{
		FSTBuffStruct newBuff = FSTBuffStruct();
		BuffMap.Add(inType, FSTBuffStruct(inType, inValue, inDuration));

		appliedBuff = BuffMap.Find(inType);
		if (appliedBuff)
		{
			GetWorld()->GetTimerManager().SetTimer(appliedBuff->Handle_Buff,
			[this, inType]()
			{
				this->ClearBuff_Server(inType);
			},
			inDuration, false);
		}
	}

	// Recalculate Status
	CalculateStatus();
	ApplyStatus();
}

void USTPlayerStatusComponent::ClearBuff_Server_Implementation(ESTBuffType inType)
{
	BuffMap.Remove(inType);
	CalculateStatus();
	ApplyStatus();
}

void USTPlayerStatusComponent::SetDefaultStatus()
{
	MaxHP = BaseMaxHP;
	HPRegen = BaseMaxHPRegen;
	MovementSpeed = BaseMovementSpeed;
	AttackSpeed = BaseAttackSpeed;
	CriticalDamagePercent = BaseCriticalDamagePercent;
	CriticalRate = BaseCriticalRate;
	DamageIncreaseRate = 0.f;
	CDR = 0.f;
}

void USTPlayerStatusComponent::CalculateStatus()
{
	if (GetOwner()->HasAuthority())
	{
		UInventorySubsystem* InvSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<UInventorySubsystem>();
		UInventoryComponent* InvComp = GetOwner()->GetComponentByClass<UInventoryComponent>();
		if (!InvComp || !InvSubSystem) return;

		// Reset Status 
		SetDefaultStatus();

		// ReCalculate Status
		const TArray<FInvSlotStruct>& Inventory = InvComp->GetInventory();
		for (auto& Slot : Inventory)
		{
			if (Slot.ItemObject)
			{
				Slot.ItemObject->CalculateStatus(GetOwner());
			}
		}

		for (auto& pair : BuffMap)
		{
			FSTBuffStruct& buff = pair.Value;
			switch (buff.BuffType)
			{
			case ESTBuffType::HP:
				
				break;
			case ESTBuffType::ATK:
				
				break;
			case ESTBuffType::DEF:
				
				break;
			case ESTBuffType::MovementSpeed:
				MovementSpeed = FMath::Clamp(MovementSpeed * (1 + buff.Value), 0, MaxMovementSpeed);
				break;
			case ESTBuffType::AttackSpeed:
				AttackSpeed = FMath::Clamp(
					AttackSpeed * (1 + buff.Value),
					0, MaxAttackSpeed);
				break;
			case ESTBuffType::DamageIncreaseRate:
				DamageIncreaseRate = FMath::Clamp(DamageIncreaseRate + buff.Value, 0, MaxDamageIncreaseRate);
				break;
			case ESTBuffType::CriticalDamagePercent:
				CriticalDamagePercent = FMath::Clamp(CriticalDamagePercent * buff.Value, 0, MaxCriticalDamagePercent);
				break;
			case ESTBuffType::CriticalRate:
				CriticalRate = FMath::Clamp(CriticalRate + buff.Value, 0, MaxCriticalRate);
				break;
			case ESTBuffType::CDR:
				CDR = FMath::Clamp(CDR * buff.Value, 0, MaxCDR);
				break;
			default:
				break;
			}
		}
	}

	ApplyStatus();
}

void USTPlayerStatusComponent::ApplyStatus()
{
	if (UCharacterMovementComponent* characterMovement = GetOwner()->GetComponentByClass<UCharacterMovementComponent>())
	{
		characterMovement->MaxWalkSpeed = MovementSpeed;
	}
}

void USTPlayerStatusComponent::OnRep_HPUpdated()
{
	UE_LOG(LogTemp, Display, TEXT("USTPlayerStatusComponent::OnRep_HPUpdated Called!"));
	// Update UI
	OnHPDelegate.Broadcast(HP, MaxHP);
	// Check Died
}

void USTPlayerStatusComponent::RegenHP()
{
	HP = FMath::Clamp(HP + HPRegen, 0, MaxHP);
	OnRep_HPUpdated();
}

