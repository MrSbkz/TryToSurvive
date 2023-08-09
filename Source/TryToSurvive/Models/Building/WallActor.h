#pragma once

#include "BuildingActorBase.h"
#include "WallActor.generated.h"

UCLASS(ClassGroup=(Custom))
class AWallActor : public ABuildingActorBase
{	
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	// virtual void DefineLocation(FVector& BuildingLocation) override;
	
	virtual bool CanBuild() override;
};