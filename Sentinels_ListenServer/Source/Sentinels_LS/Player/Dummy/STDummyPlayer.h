// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STEnums.h"
#include "STDummyPlayer.generated.h"

class USceneCaptureComponent2D;
class UMaterial;
class USkeletalMeshComponent;

UCLASS()
class SENTINELS_LS_API ASTDummyPlayer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTDummyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	const FUniqueNetIdRepl GetPlayerID() { return PlayerID; }
	void SetPlayerID(const FUniqueNetIdRepl& ID) { PlayerID = ID; }

	const bool GetbIsShow() { return bIsShow; }

	UFUNCTION(BlueprintCallable)
	void SetbIsShow(const bool value) { bIsShow = value; }

	UTextureRenderTarget2D* GetTextureRenderTarget2D();

	UMaterial* GetMaterial() { return Material; }

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetSKMeshComponent(ESKParts partName);

public:
	UFUNCTION(BlueprintCallable, Category = "Dummy Player")
	static ASTDummyPlayer* FindByID(UObject* WorldContextObject, FUniqueNetIdRepl PlayerID);

protected:
	void AddSKComponents(FName ComponentName);

protected:
	UFUNCTION()
	void OnRep_PlayerID();

	UFUNCTION()
	void OnRep_CurrentClass();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCChangeCurrentClass(ESTClassType InClass);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneCaptureComponent2D> CaptureComponent;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "SK Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SKMeshComponent;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "SkeletalMesh Components", meta = (AllowPrivateAccess = "true"))
	TArray<USkeletalMeshComponent*> SKMeshComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	TMap<ESTClassType, USkeletalMesh*> WeaponMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimInstance", meta = (AllowPrivateAccess = "true"))
	TMap<ESTClassType, TSubclassOf<UAnimInstance>> AIMap;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerID, VisibleAnywhere, BlueprintReadOnly, Category = "PlayerID", meta = (AllowPrivateAccess = "true"))
	FUniqueNetIdRepl PlayerID;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentClass, VisibleAnywhere, BlueprintReadOnly, Category = "PlayerID", meta = (AllowPrivateAccess = "true"))
	ESTClassType CurrentClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material", meta = (AllowPrivateAccess = "true"))
	UMaterial* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsShow;
};
