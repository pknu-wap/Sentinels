// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sentinels_LSGameMode.h"
#include "STGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "STGameMode_Roguelite.generated.h"

class ASTMissionSection;
class ASTEliteBase;

UCLASS()
class SENTINELS_LS_API ASTGameMode_Roguelite : public ASentinels_LSGameMode
{
	GENERATED_BODY()
	
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PreInitializeComponents() override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual void BeginPlay() override;

public:
	void TeleportPlayersToPlayerStarts(const TArray<class APlayerStart*>& inPlayerStarts);
	bool TeleportToNextSection();
	bool ActivateNextSection();

protected:
	UFUNCTION()
	void OnTeleportEnded();

/*
	Mission
*/
protected:
	void OnMissionCleared();

/*
	Elite Boss
*/
public:
	void EliteBossSpawned(ASTEliteBase* InElite);

protected:
	UFUNCTION()
	void OnEliteBossCleared(AActor* DiedEnemy);

	UFUNCTION()
	void OnEliteBossClearCallback();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SectionTimeLimit = 180.f;

	UPROPERTY()
	TArray<ASTMissionSection*> CachedSections;

	UPROPERTY()
	ASTMissionSection* CurrentSection = nullptr;

	UPROPERTY()
	int CurrentSectionIdx = -1;
};

template<typename T>
void ShuffleTArray(TArray<T>& Array)
{
    float CurrentTime = UGameplayStatics::GetRealTimeSeconds(nullptr);
    int32 Seed = UKismetMathLibrary::RandomIntegerInRange(0, 10000);

    FMath::RandInit(Seed);

    int32 NumElements = Array.Num();

    for (int32 i = NumElements - 1; i > 0; --i)
    {
        int32 RandomIndex = FMath::RandRange(0, i);

        if (i != RandomIndex)
        {
            Array.Swap(i, RandomIndex);
        }
    }
}