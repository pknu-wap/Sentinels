// Copyright Epic Games, Inc. All Rights Reserved.

#include "Sentinels_LSGameMode.h"
#include "Sentinels_LSCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASentinels_LSGameMode::ASentinels_LSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
