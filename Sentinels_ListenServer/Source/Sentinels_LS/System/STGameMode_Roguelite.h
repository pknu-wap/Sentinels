// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sentinels_LSGameMode.h"
#include "Kismet/GameplayStatics.h"
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
	void TeleportPlayersToPlayerStarts(const TArray<APlayerStart*>& inPlayerStarts);
	void TeleportToNextSection();
	bool ActivateNextSection();

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
    int32 Seed = FMath::FloorToInt(CurrentTime * 1000.0f);

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