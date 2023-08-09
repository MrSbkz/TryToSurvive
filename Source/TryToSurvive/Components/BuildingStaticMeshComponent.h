#pragma once
#include "Components/BoxComponent.h"
#include "TryToSurvive/Enums/BuildingMaterialType.h"

#include "BuildingStaticMeshComponent.generated.h"

UCLASS()
class UBuildingStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Materials")
	TArray<UMaterialInterface*> PreviewMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Materials")
	TArray<UMaterialInterface*> BaseMaterials;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> CurrentMaterials;

	virtual void SetMaterials(EBuildingMaterialType MaterialType);

	void SetupCollisions(FName CollisionName);

	void SetPreviewMaterialsColor(FLinearColor Color);
	
private:
	void SetCurrentMaterials(TArray<UMaterialInterface*> Materials);
};
