#pragma once

#include "TryToSurvive/Models/Building/BuildingActorBase.h"
#include "FBuildingMaterialPair.generated.h"

USTRUCT(BlueprintType)
struct FBuildingMaterialPair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABuildingActorBase> BuildingItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UMaterialInterface*> Preview;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UMaterialInterface*> Base;
};
