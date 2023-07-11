#pragma once
#include "TryToSurvive/Enums/BuildingMaterialType.h"
#include "TryToSurvive/Models/Building/BuildingActorBase.h"

#include "BuildingComponent.generated.h"

class ATryToSurviveCharacter;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UBuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBuildingComponent();

	virtual void BeginPlay() override;

	void SetBuildingMode();

	void ResetBuilding();

	void Build();

	void ProcessRotation() const;

	bool IsBuildingMode = false;

protected:
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Building Items")
	TArray<TSubclassOf<ABuildingActorBase>> BuildingItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Trace")
	float MaxBuildingDistance = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Trace")
	float GridSnapValue = 25.0f;

private:
	void StartPreview(const FHitResult& HitResult);

	void SetBuildingLocation(const FHitResult& HitResult);

	void CreateBuildingItem(EBuildingMaterialType MaterialType, const FHitResult& HitResult);

	void SetBuildingMaterial(EBuildingMaterialType MaterialType);

	void DrawTrace(FHitResult& HitResult, TArray<AActor*> IgnoredActors) const;

	void SetStartEndLocation(FVector& StartLocation, FVector& EndLocation) const;

	void SetCurrentMaterials(TArray<UMaterialInterface*> Materials);

	void SetPreviewMaterialsColor(FLinearColor Color);

	UPROPERTY()
	ABuildingActorBase* CurrentBuildItem;

	UPROPERTY()
	ATryToSurviveCharacter* Owner;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> CurrentMaterials;

	FVector BuildingSpawnLocation;

	bool IsRotating;

	bool IsBuildingEnable;

	FVector CurrentBuildingExtend;

	FLinearColor CurrentPreviewColor;
};
