// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interact/InteractableActor.h"
#include "STEnums.h"
#include "STPortal.generated.h"

//UENUM(BlueprintType)
//enum class ESTLEVEL1 : uint8
//{
//	StartPoint,
//	BossPoint,
//	SectionA,
//	SectionB,
//	SectionC,
//	SectionD
//};

/**
 * 
 */
UCLASS()
class SENTINELS_LS_API ASTPortal : public AInteractableActor
{
	GENERATED_BODY()

public:
	ASTPortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnStartVoting();

	UFUNCTION()
	void OnVoteUpdated(EPortalType portalType, int voteCount);

	UFUNCTION(NetMulticast, reliable)
	void OnVoteUpdated_Multicast(EPortalType portalType, int voteCount);


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USTWC_LocalPlayerFacing> FacingWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USTWC_LocalPlayerFacing* WC_World;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPortalType PortalType;
};
