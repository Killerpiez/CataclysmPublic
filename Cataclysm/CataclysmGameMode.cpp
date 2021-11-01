// Copyright Epic Games, Inc. All Rights Reserved.

#include "CataclysmGameMode.h"
#include "Base_PlayerController.h"
#include "CataclysmCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACataclysmGameMode::ACataclysmGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}