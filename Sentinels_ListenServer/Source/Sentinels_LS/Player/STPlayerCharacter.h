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
class USTPlayerStatusComponent;
class UCameraModeManagerComponent;
class UCameraShakeBase;
struct FInputActionValue;

UCLASS()
class SENTINELS_LS_API ASTPlayerCharacter : public ASTCharacterBase
{
	GENERATED_BODY()

public:
	ASTPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	/*
		APawn interface
	*/ 
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void PossessedBy(AController* NewController);

	/*
		ACharacter Interface
	*/
	virtual void Jump() override;

	/*
		On Montage Ended (For GameplayTag)
	*/
	UFUNCTION()
	void OnMontageEnded_Callback(UAnimMontage* Montage, bool bInterrupted);


	/*
		Input	
	*/
public:
	void ClearAllMappingContext();
	void BindDefaultTopDownInput();
	void BindDefaultThirdPersonInput();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);


	/*
		Step
	*/
protected:
	bool CanDoStep() const;

	void Step_Pressed();
	void PlayMontage_Step();

	UFUNCTION(Server, Reliable)
	virtual void Step_Pressed_Server();

	UFUNCTION(NetMulticast, Reliable)
	virtual void Step_Pressed_Multicast();


	/*
		Normal Attack
	*/
protected:
	void BindAttackDelegate();

	bool CanDoAttack() const;
	virtual void NormalAttack_Pressed();
	void PlayMontage_NormalAttack(int currentCombo);

	UFUNCTION(Server, Reliable)
	virtual void NormalAttack_Pressed_Server(int currentCombo);

	UFUNCTION(NetMulticast, Reliable)
	virtual void NormalAttack_Pressed_Multicast(int currentCombo);

	UFUNCTION()
	void StartCheckNextInput();

	UFUNCTION()
	void CheckNextAttack();

	void ResetAttackInfo();

protected:
	int CurrentCombo = 0;
	int MaxCombo = 3;
	bool bIsCheckingNextInput = false;
	bool bShouldDoNextAttack = false;


	/*
		Skills
	*/
protected:
	bool CanDoSkill() const;
	bool IsSkillMontage(const UAnimMontage* inMontage) const;

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

	virtual void Skill_Passive_Pressed();
	void PlayMontage_Skill_Passive();

	UFUNCTION(Server, Reliable)
	virtual void Skill_Passive_Pressed_Server();

	UFUNCTION(NetMulticast, Reliable)
	virtual void Skill_Passive_Pressed_Multicast();

	/*
		Damage (Server)
	*/
public:
	UFUNCTION(BlueprintCallable)
	virtual void AdjustFinalDamage(float& DamageAmount, FDamageEvent const& DamageEvent, AActor* DamagedActor);

protected:

	
	/*
		On Attack Success 
	*/
public:
	UFUNCTION(Server, Reliable)
	virtual void OnAttackSuccess_Server(float DamageAmount, FDamageEvent const& DamageEvent, AActor* DamagedActor);

	UFUNCTION(Client, Reliable)
	virtual void OnAttackSuccess_Client();


	/*
		Time Dilation
	*/
public:
	UFUNCTION(Client, Reliable)
	void ApplyCustomTimeDilation(float inValue, float inDuration);

protected:
	FTimerHandle Handle_TimeDilation;

	/*
		Camera Shake
	*/
public:
	UFUNCTION(Client, Reliable)
	void ApplyAttackCameraShake();

	UPROPERTY(EditAnywhere, Category = Camera)
	TSubclassOf<UCameraShakeBase> CameraShakeClass_Attack;


protected:
	/*
		Camera
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraModeManagerComponent* CameraManager;

	/*
		Inventory
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	/*
		Status
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	TObjectPtr<USTPlayerStatusComponent> StatusComponent;

	/* 
		Input 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* NormalAttack_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Step_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Skill_Q_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Skill_W_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Skill_E_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Skill_R_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Skill_Passive_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContext_ThirdPerson;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/*
		Montages
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_NormalAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_Step_F;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_Skill_Q;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_Skill_W;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_Skill_E;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_Skill_R;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage_Skill_Passive;
};
