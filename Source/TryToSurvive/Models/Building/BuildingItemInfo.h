#pragma once

#include "BuildingActorBase.h"

#include "BuildingItemInfo.generated.h"

USTRUCT(BlueprintType)
struct FResourcesInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MinAmount;
};

USTRUCT(BlueprintType)
struct FBuildingItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABuildingActorBase> BuildingItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FResourcesInfo> NeededResources;
};
