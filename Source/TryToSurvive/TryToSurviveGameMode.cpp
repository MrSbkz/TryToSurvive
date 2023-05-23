// Copyright Epic Games, Inc. All Rights Reserved.

#include "TryToSurviveGameMode.h"
#include "TryToSurviveCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATryToSurviveGameMode::ATryToSurviveGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
