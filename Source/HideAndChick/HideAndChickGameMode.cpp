// Copyright Epic Games, Inc. All Rights Reserved.

#include "HideAndChickGameMode.h"
#include "HideAndChickCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHideAndChickGameMode::AHideAndChickGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
