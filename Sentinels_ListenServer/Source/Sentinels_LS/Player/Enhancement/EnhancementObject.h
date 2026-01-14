// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/NetworkObject.h"
#include "STGameplayTags.h"
#include "EnhancementObject.generated.h"

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API UEnhancementObject : public UNetworkObject
{
	GENERATED_BODY()
	
/*
	UObject Interface
*/
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

/*
	Normal Status
*/
public:
	UFUNCTION(BlueprintImplementableEvent)
	void CalculateStatus(class AActor* OwnerActor);

public:
	FGameplayTag GetEnhancementTag() const { return EnhancementTag; }
	void AddQuantity(int quantity) { Quantity += 1; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EnhancementTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Quantity = 1;
};
