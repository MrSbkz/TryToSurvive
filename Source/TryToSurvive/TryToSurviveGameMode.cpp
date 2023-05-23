#include "TryToSurviveGameMode.h"
#include "TryToSurviveCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATryToSurviveGameMode::ATryToSurviveGameMode()
	: Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
