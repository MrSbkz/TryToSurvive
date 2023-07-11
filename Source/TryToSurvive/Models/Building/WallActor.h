#pragma once

#include "BuildingActorBase.h"
#include "WallActor.generated.h"

UCLASS(ClassGroup=(Custom))
class AWallActor : public ABuildingActorBase
{	
	GENERATED_BODY()

public:
	virtual bool SetLocation(FVector& BuildingLocation) override;

protected:
	virtual void BeginPlay() override;
	
private:
	bool CanBuild();
};