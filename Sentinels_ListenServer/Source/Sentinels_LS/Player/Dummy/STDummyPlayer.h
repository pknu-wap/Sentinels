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

	const FString GetPlayerName() { return PlayerName; }
	void SetPlayerName(const FString& name) { PlayerName = name; }

	UTextureRenderTarget2D* GetTextureRenderTarget2D();

	UMaterial* GetMaterial() { return Material; }

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetSKMeshComponent(ESKParts partName);

protected:
	UFUNCTION()
	void OnRep_PlayerID();

	UFUNCTION()
	void OnRep_CurrentClass();

	UFUNCTION()
	void OnRep_SKName();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCChangeCurrentClass(ESTClassType InClass);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPCChangeSKMesh(ESKParts Part, FName SKMeshRowName);

	UFUNCTION(BlueprintCallable)
	void ChangeSKMeshName(ESKParts Part, FName SKMeshRowName);

protected:
	/*
		CaptureComponent
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneCaptureComponent2D> CaptureComponent;

	/*
		SkeletalMesh & Animation
	*/

	UPROPERTY(ReplicatedUsing = OnRep_PlayerID, EditAnywhere, BlueprintReadWrite, Category = "SK Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SKMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> HeadComponent;
	UPROPERTY(ReplicatedUsing = OnRep_SKName, EditAnywhere, BlueprintReadWrite, Category = "SK Row Name", meta = (AllowPrivateAccess = "true"))
	FName Name_Head;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> HoodComponent;
	UPROPERTY(ReplicatedUsing = OnRep_SKName, EditAnywhere, BlueprintReadWrite, Category = "SK Row Name", meta = (AllowPrivateAccess = "true"))
	FName Name_Hood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> LongHairComponent;
	UPROPERTY(ReplicatedUsing = OnRep_SKName, EditAnywhere, BlueprintReadWrite, Category = "SK Row Name", meta = (AllowPrivateAccess = "true"))
	FName Name_LongHair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> GlassesComponent;
	UPROPERTY(ReplicatedUsing = OnRep_SKName, EditAnywhere, BlueprintReadWrite, Category = "SK Row Name", meta = (AllowPrivateAccess = "true"))
	FName Name_Glasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> UpperBodyComponent;
	UPROPERTY(ReplicatedUsing = OnRep_SKName, EditAnywhere, BlueprintReadWrite, Category = "SK Row Name", meta = (AllowPrivateAccess = "true"))
	FName Name_UpperBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> BackpackComponent;
	UPROPERTY(ReplicatedUsing = OnRep_SKName, EditAnywhere, BlueprintReadWrite, Category = "SK Row Name", meta = (AllowPrivateAccess = "true"))
	FName Name_Backpack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Hand_LComponent;
	UPROPERTY(ReplicatedUsing = OnRep_SKName, EditAnywhere, BlueprintReadWrite, Category = "SK Row Name", meta = (AllowPrivateAccess = "true"))
	FName Name_Hand_L;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Hand_RComponent;
	UPROPERTY(ReplicatedUsing = OnRep_SKName, EditAnywhere, BlueprintReadWrite, Category = "SK Row Name", meta = (AllowPrivateAccess = "true"))
	FName Name_Hand_R;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> BottomBodyComponent;
	UPROPERTY(ReplicatedUsing = OnRep_SKName, EditAnywhere, BlueprintReadWrite, Category = "SK Row Name", meta = (AllowPrivateAccess = "true"))
	FName Name_BottomBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> FeetComponent;
	UPROPERTY(ReplicatedUsing = OnRep_SKName, EditAnywhere, BlueprintReadWrite, Category = "SK Row Name", meta = (AllowPrivateAccess = "true"))
	FName Name_Feet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> GreatSwordComponent;
	UPROPERTY(ReplicatedUsing = OnRep_SKName, EditAnywhere, BlueprintReadWrite, Category = "SK Row Name", meta = (AllowPrivateAccess = "true"))
	FName Name_GreatSword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> KatanaComponent;
	UPROPERTY(ReplicatedUsing = OnRep_SKName, EditAnywhere, BlueprintReadWrite, Category = "SK Row Name", meta = (AllowPrivateAccess = "true"))
	FName Name_Katana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Blade_LComponent;
	UPROPERTY(ReplicatedUsing = OnRep_SKName, EditAnywhere, BlueprintReadWrite, Category = "SK Row Name", meta = (AllowPrivateAccess = "true"))
	FName Name_Blade_L;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Blade_RComponent;
	UPROPERTY(ReplicatedUsing = OnRep_SKName, EditAnywhere, BlueprintReadWrite, Category = "SK Row Name", meta = (AllowPrivateAccess = "true"))
	FName Name_Blade_R;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	TMap<ESTClassType, USkeletalMesh*> WeaponMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimInstance", meta = (AllowPrivateAccess = "true"))
	TMap<ESTClassType, TSubclassOf<UAnimInstance>> AIMap;


	/*
		ETC
	*/

	UFUNCTION()
	void OnRep_PlayerNames();

	UPROPERTY(ReplicatedUsing = OnRep_PlayerID, VisibleAnywhere, BlueprintReadOnly, Category = "PlayerID", meta = (AllowPrivateAccess = "true"))
	FUniqueNetIdRepl PlayerID;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerNames, VisibleAnywhere, BlueprintReadOnly, Category = "PlayerID", meta = (AllowPrivateAccess = "true"))
	FString PlayerName;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentClass, VisibleAnywhere, BlueprintReadOnly, Category = "PlayerID", meta = (AllowPrivateAccess = "true"))
	ESTClassType CurrentClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material", meta = (AllowPrivateAccess = "true"))
	UMaterial* Material;
};
