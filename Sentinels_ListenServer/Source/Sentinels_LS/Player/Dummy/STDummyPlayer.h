// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STEnums.h"
#include "STDummyPlayer.generated.h"

class USceneCaptureComponent2D;

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

	UFUNCTION(BlueprintCallable)
	void UpdateSkeletalMesh(ESTClassType InClass);

public:
	const FUniqueNetIdRepl GetPlayerID() { return PlayerID; }
	void SetPlayerID(const FUniqueNetIdRepl& ID) { PlayerID = ID; }

	UTextureRenderTarget2D* GetTextureRenderTarget2D();

public:
	UFUNCTION(BlueprintCallable, Category = "Dummy Player")
	static ASTDummyPlayer* FindByID(UWorld* World, FUniqueNetIdRepl PlayerID);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneCaptureComponent2D> CaptureComponent;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "SkeletalMesh Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SKComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	TMap<ESTClassType, USkeletalMesh*> SKMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimInstance", meta = (AllowPrivateAccess = "true"))
	TMap<ESTClassType, TSubclassOf<UAnimInstance>> AIMap;

	// IsValid를 이용해서 이미 누가 가져갔는지 확인해서 할당
	// ReplicatedUsing을 사용해야할까?
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "PlayerID", meta = (AllowPrivateAccess = "true"))
	FUniqueNetIdRepl PlayerID;
};
