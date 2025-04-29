// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interact/InteractableCharacter.h"
#include "InteractableTank.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class UTankAnimInstance;
class AProjectileBase;

UENUM()
enum ETankCameraMode
{
	ETC_Normal,
	ETC_Focus,
}; 

UCLASS()
class SENTINELS_LS_API AInteractableTank : public AInteractableCharacter
{
	GENERATED_BODY()

public:
	AInteractableTank();

public:
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	virtual void PossessedBy(AController* NewController);

	UFUNCTION(NetMulticast, Reliable)
	void PossessedCallback_Multicast();
	

/*
	Interact Interface
*/
protected:
	virtual void Interact_Implementation(UInteractComponent* InteractingComponent) override;
	virtual void Interact_Finish_Implementation(UInteractComponent* InteractingComponent) override;


/*
*	Move & Look
*/
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);


/*
	Tank Fire	
*/
protected:
	void Fire_Pressed();

	UFUNCTION(Server, Reliable)
	void Fire_Pressed_Server();


/*
	Convert Fire Mode
*/
protected:
	void ConvertMode_Pressed();



/*
*	Get Off
*/
protected:
	void GetOffFromTank();

	UFUNCTION(Server, Reliable)
	void GetOffFromTank_Server();

protected:
	UPROPERTY(EditAnywhere)
	USceneComponent* GetOffSceneComponent;

	UPROPERTY()
	APlayerController* CachedPC;

	UPROPERTY()
	APawn* CachedPlayerPawn;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContext_Tank;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ConvertModeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GetOffAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> RocketSubClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> CrossHairWidgetClass;

protected:
	UPROPERTY()
	UTankAnimInstance* TankAnimInst;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float WheelSpeedOffset;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float WheelSpeed;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float TurretAngle;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float GunElevation;

private:
	UPROPERTY()
	UUserWidget* CrossHairWidget;

	ETankCameraMode CurrentCameraMode = ETankCameraMode::ETC_Normal;

	UPROPERTY()
	FVector CameraLocation_Normal;
};
