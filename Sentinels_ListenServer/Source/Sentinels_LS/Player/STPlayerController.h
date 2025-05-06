// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "STEnums.h"
#include "STPlayerController.generated.h"

class ASTPlayerCharacter;
class ASTPlayerState;
class UInputAction;
class USplineComponent;
class UInventoryComponent;
class USkillComponent;
class UInteractComponent;
class USTPlayerUIComponent;

class FOnlineSessionSearch;
class FOnlineSessionSearchResult;

UCLASS()
class SENTINELS_LS_API ASTPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASTPlayerController();

protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

/*
	Input
*/
	virtual void SetupInputComponent() override;

public:
	void BindDefaultTopDownInput();
	void BindDefaultThirdPersonInput();

/*
	Update Player
*/
public:
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void UpdatePlayerClass(ESTClassType InClass);

	TSubclassOf<APawn> GetDefaultPlayerClass() { return DefaultPlayerClass; }


/*
	Move
*/
protected:
	void MoveClick_Started();
	void MoveClick_Triggered();
	void MoveClick_Released();
	void AutoRun();

/*
	UI
*/
public:
	USTPlayerUIComponent* GetUIComponent() { return UIComponent; }

/*
	Interact
*/
protected:
	void Interact_Pressed();
	void Interact_Released();
	


public:
/*
	Session
*/
	void RegisterSelfToSession(FName SessionName);

	UFUNCTION(Server, Reliable)
	void RegisterSelfToSession_Server(FName SessionName);

	FName CurrentSession;

protected:
/*
	Input
*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

/*
	Movement
*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInteractComponent> InteractComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USkillComponent> SkillComponent;

/*
	Classes
*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APawn> PawnClass_GreatSword;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APawn> PawnClass_Katana;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APawn> PawnClass_DualBlade;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APawn> PawnClass_Magician;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APawn> DefaultPlayerClass;

	/*
		UI
	*/
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USTPlayerUIComponent> UIComponent;

private:
	FTimerHandle Handle_AutoRun;
	bool bAutoRun = false;
	FVector CachedDestination;
	float FollowTime; // For how long it has been pressed

};
