// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"
#include "STDamageTypes.generated.h"

USTRUCT()
struct FSTPointDamageEvent : public FPointDamageEvent
{
    GENERATED_BODY()

	FSTPointDamageEvent() {};

	FSTPointDamageEvent(bool InbIsCritical, float InDamage, const FHitResult& InHitInfo, FVector const& InShotDirection, TSubclassOf<UDamageType> InDamageTypeClass)
		: bIsCritical(InbIsCritical), FPointDamageEvent(InDamage, InHitInfo, InShotDirection, InDamageTypeClass)
	{}

	/** ID for this class. NOTE this must be unique for all damage events. */
	static const int32 ClassID = 15;

	virtual int32 GetTypeID() const override { return FSTPointDamageEvent::ClassID; };
	virtual bool IsOfType(int32 InID) const override { return (FSTPointDamageEvent::ClassID == InID) || FDamageEvent::IsOfType(InID); };

public:
    bool bIsCritical = false;
};

UCLASS()
class SENTINELS_LS_API USTBaseDamageType : public UDamageType
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StunnedTime = 1.f;
};

UCLASS()
class SENTINELS_LS_API USTKatanaDamageType : public USTBaseDamageType
{
	GENERATED_BODY()

public:
};
