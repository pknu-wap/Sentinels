// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/STCharacterBase.h"
#include "STPlayerCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;
class ASTPlayerCharacter;
class UInventoryComponent;

UCLASS()
class SENTINELS_LS_API ASTPlayerCharacter : public ASTCharacterBase
{
	GENERATED_BODY()

public:
	ASTPlayerCharacter();

protected:
	// APawn interface
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/*
		Skills
	*/
	virtual void Skill_Q_Pressed();
	void PlayMontage_Skill_Q();

	UFUNCTION(Server, Reliable)
	virtual void Skill_Q_Pressed_Server();

	UFUNCTION(NetMulticast, Reliable)
	virtual void Skill_Q_Pressed_Multicast();

	virtual void Skill_W_Pressed();
	void PlayMontage_Skill_W();

	UFUNCTION(Server, Reliable)
	virtual void Skill_W_Pressed_Server();

	UFUNCTION(NetMulticast, Reliable)
	virtual void Skill_W_Pressed_Multicast();

	virtual void Skill_E_Pressed();
	void PlayMontage_Skill_E();

	UFUNCTION(Server, Reliable)
	virtual void Skill_E_Pressed_Server();

	UFUNCTION(NetMulticast, Reliable)
	virtual void Skill_E_Pressed_Multicast();

	virtual void Skill_R_Pressed();
	void PlayMontage_Skill_R();

	UFUNCTION(Server, Reliable)
	virtual void Skill_R_Pressed_Server();

	UFUNCTION(NetMulticast, Reliable)
	virtual void Skill_R_Pressed_Multicast();

protected:
	/*
		Camera
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/*
		Inventory
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	/* 
		Input 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Skill_Q_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Skill_W_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Skill_E_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Skill_R_Action;

	/*
		Montages
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_Skill_Q;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_Skill_W;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_Skill_E;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_Skill_R;
};
