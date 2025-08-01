// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interact/InteractableActor.h"
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
	virtual void Interact_Implementation(UInteractComponent* InteractingComponent) override;
	virtual void Interact_Finish_Implementation(UInteractComponent* InteractingComponent) override;
	virtual void ShowInteractiveUI_Implementation(UInteractComponent* InteractingComponent) override;
	virtual void HideInteractiveUI_Implementation(UInteractComponent* InteractingComponent) override;

protected:
	void Activate();
	void Deactivate();

	void Teleport(AActor* Target, FVector& Location);
protected:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCUpdate();

	UFUNCTION(Server, Reliable)
	void ServerRPCVote();

protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int CurrentVoteCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FDataTableRowHandle LevelHandle_DataRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USTWC_LocalPlayerFacing* WC_World;
};
