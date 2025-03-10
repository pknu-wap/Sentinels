// Copyright Epic Games, Inc. All Rights Reserved.

#include "SentinelsGameMode.h"
#include "SentinelsCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASentinelsGameMode::ASentinelsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
