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

USTRUCT()
struct FPlayerSKMeshesPartsName
{
	GENERATED_BODY()

	UPROPERTY()
	FName Name_Head = "0";
	UPROPERTY()
	FName Name_Hood = "0";
	UPROPERTY()
	FName Name_LongHair = "0";
	UPROPERTY()
	FName Name_Glasses = "0";
	UPROPERTY()
	FName Name_UpperBody = "0";
	UPROPERTY()
	FName Name_Backpack = "0";
	UPROPERTY()
	FName Name_Hand_L = "0";
	UPROPERTY()
	FName Name_Hand_R = "0";
	UPROPERTY()
	FName Name_BottomBody = "0";
	UPROPERTY()
	FName Name_Feet = "0";
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPassiveSkillReadyStateChanged, bool, IsReady);

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

public:
	/*
		Customize SKMesh Parts
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSKMeshParts(FPlayerSKMeshesRowName PlayerSKMeshesRowName);

	/*
		Input	
	*/
public:
	UFUNCTION(BlueprintCallable)
	void ClearAllMappingContext();

	UFUNCTION(BlueprintCallable)
	void BindDefaultTopDownInput();

	UFUNCTION(BlueprintCallable)
	void BindDefaultThirdPersonInput();

public:
	UFUNCTION(BlueprintImplementableEvent)
	void TeleportWithDissolve(const FTransform& InTransform);

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);


	/*
		Run
	*/
protected:
	void Run_Pressed();
	void Run_Released();

	UFUNCTION(Server, Reliable)
	void Run_Pressed_Server();

	UFUNCTION(Server, Reliable)
	void Run_Released_Server();


	/*
		Step
	*/
protected:
	bool CanDoStep() const;

	void Step_Pressed();
	void PlayMontage_Step();

	UFUNCTION()
	void SetWarpTarget_Step();

	UFUNCTION(Server, Reliable)
	virtual void Step_Pressed_Server();

	UFUNCTION(NetMulticast, Reliable)
	virtual void Step_Pressed_Multicast();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StepDistance = 200.f;


	/*
		Normal Attack
	*/
protected:
	void BindAttackDelegate();

	bool CanDoAttack() const;
	virtual void NormalAttack_Pressed();
	virtual void NormalAttack_Released();
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
	bool bIsAttackPressinng = false;


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

protected:
	UPROPERTY(BlueprintAssignable)
	FOnPassiveSkillReadyStateChanged OnPassiveSkillReadyStateChanged;

	/*
		Damage (Server)
	*/
public:
	UFUNCTION(BlueprintCallable)
	virtual void ApplyDamageToActor(float DamagePercent, TSubclassOf<UDamageType> DamageType, AActor* DamagedActor);

	UFUNCTION(BlueprintCallable)
	virtual void AdjustFinalDamage(float& DamageAmount, FDamageEvent const& DamageEvent, AActor* DamagedActor);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UDamageType> BaseDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UDamageType> CriticalDamageType;
	
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

	/*
	Customize
	*/
public:
	UFUNCTION()
	void OnRep_SKMeshPartsName();

	void ConvertRowNametoPartsName(FPlayerSKMeshesRowName RowName);
	void ConvertPartsNametoRowName(FPlayerSKMeshesRowName& RowName);

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
		Data
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* DataTable_Skill;

	/* 
		Input 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;

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


	/*
	Customize
	*/
	UPROPERTY(ReplicatedUsing = OnRep_SKMeshPartsName)
	FPlayerSKMeshesPartsName SKMeshPartsName;
};
