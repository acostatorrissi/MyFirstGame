// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyFirstGameGameMode.h"
#include "MyFirstGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMyFirstGameGameMode::AMyFirstGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
