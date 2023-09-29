#include "TryToSurviveGameMode.h"
#include "TryToSurviveCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATryToSurviveGameMode::ATryToSurviveGameMode()
	: Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/MainCharacter/Blueprints/BP_Warrior"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
