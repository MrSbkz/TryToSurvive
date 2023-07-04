#pragma once

#include "TryToSurvive/Models/ActorWithHealthBase.h"
#include "BuildingActorBase.generated.h"

UCLASS(ClassGroup=(Custom))
class ABuildingActorBase : public AActorWithHealthBase
{	
	GENERATED_BODY()

public:
	ABuildingActorBase()
	{
		MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BC_MeshComponent");
		SetRootComponent(MeshComponent);
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* MeshComponent;
};
