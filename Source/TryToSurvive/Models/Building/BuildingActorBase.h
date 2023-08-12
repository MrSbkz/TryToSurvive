#pragma once

#include "TryToSurvive/Components/BuildingStaticMeshComponent.h"
#include "TryToSurvive/Enums/BuildingMaterialType.h"
#include "TryToSurvive/Models/ActorWithHealthBase.h"
#include "BuildingActorBase.generated.h"

UCLASS(ClassGroup=(Custom))
class ABuildingActorBase : public AActorWithHealthBase
{	
	GENERATED_BODY()

public:
	ABuildingActorBase();

	bool SetLocation(FVector& BuildingLocation);

	virtual void SetBuildingMaterials(const EBuildingMaterialType MaterialType);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UBuildingStaticMeshComponent* MeshComponent;

protected:
	virtual void SetPreviewMaterialsColor();

	virtual void DefineLocation(FVector& BuildingLocation);

	virtual bool CanBuild(){ return false; };

	FLinearColor CurrentPreviewColor = FLinearColor::White;

	bool IsBuildingEnable;
};
