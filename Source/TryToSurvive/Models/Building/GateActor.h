#pragma once

#include "CoreMinimal.h"
#include "WallActor.h"
#include "GateActor.generated.h"

UCLASS()
class TRYTOSURVIVE_API AGateActor : public AWallActor
{
	GENERATED_BODY()
public:
	AGateActor();

	virtual void SetBuildingMaterials(const EBuildingMaterialType MaterialType) override;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void SetPreviewMaterialsColor() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UBuildingStaticMeshComponent* LeftDoorComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UBuildingStaticMeshComponent* RightDoorComponent;
};
