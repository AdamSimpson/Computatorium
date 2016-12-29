// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Computatorium.h"
#include "ComputatoriumGameMode.h"
#include "ComputatoriumPlayerController.h"
#include "ComputatoriumCharacter.h"

AComputatoriumGameMode::AComputatoriumGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AComputatoriumPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}