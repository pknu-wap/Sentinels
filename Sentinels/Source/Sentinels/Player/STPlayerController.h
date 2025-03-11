// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STPlayerController.generated.h"

class UInputAction;
class USplineComponent;

UCLASS()
class SENTINELS_API ASTPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASTPlayerController();

protected:
	virtual void Tick(float DeltaTime) override;

	/*
		Input
	*/
	virtual void SetupInputComponent() override;

	/*
		Move
	*/
	void MoveClick_Started();
	void MoveClick_Triggered();
	void MoveClick_Released();

	void AutoRun();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveClickAction;

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

private:
	FTimerHandle Handle_AutoRun;
	bool bAutoRun = false;
	FVector CachedDestination;
	float FollowTime; // For how long it has been pressed
};
