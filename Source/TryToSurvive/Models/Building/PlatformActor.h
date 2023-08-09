#pragma once

#include "CoreMinimal.h"
#include "BuildingActorBase.h"
#include "PlatformActor.generated.h"


UCLASS()
class TRYTOSURVIVE_API APlatformActor : public ABuildingActorBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual bool CanBuild() override;
	
};
